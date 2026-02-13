#!/usr/bin/env bash
# Intel HEX editor driven by a YAML map file
# Usage: ./make_led_hex.sh -map <mapfile.yaml> -hex <firmware.hex>

set -e

# ---------------------------------------------------------------
# Argument parsing
# ---------------------------------------------------------------
MAP_FILE=""
HEX_FILE=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -map) MAP_FILE="$2"; shift 2 ;;
        -hex) HEX_FILE="$2"; shift 2 ;;
        *) echo "Unknown option: $1"; echo "Usage: $0 -map <map.yaml> -hex <file.hex>"; exit 1 ;;
    esac
done

if [[ -z "$MAP_FILE" || -z "$HEX_FILE" ]]; then
    echo "Usage: $0 -map <map.yaml> -hex <file.hex>"
    exit 1
fi

for f in "$MAP_FILE" "$HEX_FILE"; do
    if [[ ! -f "$f" ]]; then echo "Error: file not found: $f"; exit 1; fi
done

# ---------------------------------------------------------------
# Parse Intel HEX file into MEM[0..65535]
# Supports: data (00), extended segment (02), extended linear (04)
# ---------------------------------------------------------------
declare -a MEM
MEM_SIZE=65536
for (( i=0; i<MEM_SIZE; i++ )); do MEM[$i]=0; done

HEX_MAX_ADDR=0   # track highest address seen, to scope the save

parse_hex() {
    local ext_addr=0
    while IFS= read -r line <&3; do
        line="${line//$'\r'/}"
        [[ "$line" =~ ^:([0-9A-Fa-f]{2})([0-9A-Fa-f]{4})([0-9A-Fa-f]{2})([0-9A-Fa-f]*) ]] || continue
        local byte_count=$(( 16#${BASH_REMATCH[1]} ))
        local addr=$(( 16#${BASH_REMATCH[2]} ))
        local rec_type="${BASH_REMATCH[3]}"
        local data="${BASH_REMATCH[4]}"
        data="${data:0:$(( byte_count * 2 ))}"
        case "$rec_type" in
            00)
                local full_addr=$(( ext_addr + addr ))
                local end_addr=$(( full_addr + byte_count - 1 ))
                (( end_addr > HEX_MAX_ADDR )) && HEX_MAX_ADDR=$end_addr
                for (( i=0; i<byte_count; i++ )); do
                    local b=$(( 16#${data:$(( i*2 )):2} ))
                    local idx=$(( full_addr + i ))
                    if (( idx < MEM_SIZE )); then MEM[$idx]=$b; fi
                done ;;
            02) ext_addr=$(( 16#${data:0:4} * 16 )) ;;
            04) ext_addr=$(( 16#${data:0:4} << 16 )) ;;
        esac
    done 3< "$1"
}

parse_hex "$HEX_FILE"

# ---------------------------------------------------------------
# YAML map parser — pure bash, handles the schema:
#
# PaletteName:
#   Start: 0xNN
#   End:   0xNN
#   Entries:
#     - EntryName:
#         Size:  N
#         Start: 0xNN   <- offset relative to palette Start
#
# Populates parallel arrays indexed by palette and entry.
# ---------------------------------------------------------------
declare -a PAL_NAMES PAL_START PAL_END
declare -a ENT_PAL ENT_NAME ENT_SIZE ENT_ABS_START

parse_yaml_map() {
    local pal_idx=-1
    local pal_start=0
    local in_entries=0
    local pending_entry=""
    local pending_ent_idx=-1

    while IFS= read -r raw <&3; do
        local line="${raw//$'\r'/}"
        local stripped="${line#"${line%%[! ]*}"}"
        local indent=$(( ${#line} - ${#stripped} ))
        local content="$stripped"

        [[ -z "$content" || "$content" == \#* ]] && continue

        # Palette name: indent 0, "Name:"
        if (( indent == 0 )) && [[ "$content" =~ ^([^:]+):[[:space:]]*$ ]]; then
            pal_idx=$(( pal_idx + 1 ))
            PAL_NAMES[$pal_idx]="${BASH_REMATCH[1]}"
            in_entries=0; pending_entry=""; pending_ent_idx=-1
            continue
        fi

        # Palette-level keys: indent 2
        if (( indent == 2 && pal_idx >= 0 )); then
            if [[ "$content" =~ ^Start:[[:space:]]*(0[xX][0-9A-Fa-f]+|[0-9]+) ]]; then
                PAL_START[$pal_idx]=$(( ${BASH_REMATCH[1]} )); pal_start=${PAL_START[$pal_idx]}; continue
            fi
            if [[ "$content" =~ ^End:[[:space:]]*(0[xX][0-9A-Fa-f]+|[0-9]+) ]]; then
                PAL_END[$pal_idx]=$(( ${BASH_REMATCH[1]} )); continue
            fi
            if [[ "$content" == "Entries:" ]]; then
                in_entries=1; continue
            fi
        fi

        # Entry list item: indent 4, "- EntryName:"
        if (( indent == 4 && in_entries )) && [[ "$content" =~ ^-[[:space:]]+(.+):[[:space:]]*$ ]]; then
            pending_entry="${BASH_REMATCH[1]}"
            pending_ent_idx=${#ENT_NAME[@]}
            ENT_PAL[$pending_ent_idx]=$pal_idx
            ENT_NAME[$pending_ent_idx]="$pending_entry"
            ENT_SIZE[$pending_ent_idx]=0
            ENT_ABS_START[$pending_ent_idx]=0
            continue
        fi

        # Entry sub-keys: indent 8
        if (( indent == 8 && in_entries && pending_ent_idx >= 0 )); then
            if [[ "$content" =~ ^Size:[[:space:]]*([0-9]+) ]]; then
                ENT_SIZE[$pending_ent_idx]="${BASH_REMATCH[1]}"; continue
            fi
            if [[ "$content" =~ ^Start:[[:space:]]*(0[xX][0-9A-Fa-f]+|[0-9]+) ]]; then
                ENT_ABS_START[$pending_ent_idx]=$(( pal_start + ${BASH_REMATCH[1]} ))
                pending_ent_idx=-1
                continue
            fi
        fi

    done 3< "$1"
}

parse_yaml_map "$MAP_FILE"

PAL_COUNT=${#PAL_NAMES[@]}
if (( PAL_COUNT == 0 )); then
    echo "Error: no palettes found in $MAP_FILE"; exit 1
fi

# ---------------------------------------------------------------
# MEM read / write helpers
# ---------------------------------------------------------------
read_bytes_hex() {
    local addr=$1 size=$2 result=""
    for (( i=0; i<size; i++ )); do
        result+=$(printf '%02X' "${MEM[$(( addr+i ))]}")
    done
    printf '%s' "$result"
}

write_bytes_hex() {
    local addr=$1 size=$2 hex="$3"
    for (( i=0; i<size; i++ )); do
        MEM[$(( addr+i ))]=$(( 16#${hex:$(( i*2 )):2} ))
    done
}

# ---------------------------------------------------------------
# Interactive prompt for one map entry
# ---------------------------------------------------------------
ask_entry() {
    local label="$1" addr=$2 size=$3
    local expected_chars=$(( size * 2 ))
    local current; current=$(read_bytes_hex "$addr" "$size")
    local end_addr=$(( addr + size - 1 ))
    local addr_str; addr_str=$(printf '0x%02X-0x%02X' "$addr" "$end_addr")
    local val

    while true; do
        read -r -p "$(printf '%-36s' "$label ($addr_str)")  [current: $current]: " val || true
        val="${val//$'\r'/}"                          # strip any CR
        val="${val:-$current}"; val="${val^^}"; val="${val#0X}"
        if [[ ${#val} -eq $expected_chars ]] && [[ "$val" =~ ^[0-9A-F]+$ ]]; then
            write_bytes_hex "$addr" "$size" "$val"
            return
        fi
        echo "  !! Enter exactly $expected_chars hex digits (${size}-byte value)"
    done
}

# ---------------------------------------------------------------
# Edit all entries for a given palette index
# ---------------------------------------------------------------
edit_palette() {
    local pal_idx=$1
    printf '\nEditing: %s  (0x%02X – 0x%02X)\n' \
        "${PAL_NAMES[$pal_idx]}" "${PAL_START[$pal_idx]}" "${PAL_END[$pal_idx]}"
    echo "Press ENTER to keep the current value."

    local found=0
    for (( ei=0; ei<${#ENT_NAME[@]}; ei++ )); do
        if (( ENT_PAL[$ei] == pal_idx )); then
            ask_entry "${ENT_NAME[$ei]}" "${ENT_ABS_START[$ei]}" "${ENT_SIZE[$ei]}"
            found=1
        fi
    done
    if (( found == 0 )); then echo "  (no entries defined for this palette)"; fi
}

# ---------------------------------------------------------------
# Intel HEX write helpers
# ---------------------------------------------------------------
checksum() {
    local sum=0
    for v in "$@"; do (( sum += v )); done
    printf '%02X' $(( (256 - (sum & 0xFF)) & 0xFF ))
}

hex_record() {
    local addr=$1; shift
    local bytes=("$@") byte_count=${#bytes[@]}
    local cs_args=( "$byte_count" $(( (addr>>8)&0xFF )) $(( addr&0xFF )) 0 "${bytes[@]}" )
    local cs; cs=$(checksum "${cs_args[@]}")
    printf ':%02X%04X00' "$byte_count" "$addr"
    for b in "${bytes[@]}"; do printf '%02X' "$b"; done
    printf '%s\n' "$cs"
}

save_hex() {
    cp "$HEX_FILE" "${HEX_FILE}.bak"
    local record_size=32
    # Round HEX_MAX_ADDR up to the next full record boundary
    local save_bytes=$(( ( (HEX_MAX_ADDR + record_size) / record_size ) * record_size ))
    local total_records=$(( save_bytes / record_size ))
    {
        for (( row=0; row<total_records; row++ )); do
            local base=$(( row * record_size ))
            local row_bytes=()
            for (( col=0; col<record_size; col++ )); do
                row_bytes+=( "${MEM[$(( base + col ))]}" )
            done
            hex_record "$base" "${row_bytes[@]}"
        done
        echo ":00000001FF"
    } > "$HEX_FILE"
    echo "Backup saved to: ${HEX_FILE}.bak"
    echo "File saved to:   $HEX_FILE"
}

# ---------------------------------------------------------------
# Main loop
# ---------------------------------------------------------------
echo "HEX Editor — hex: $HEX_FILE  map: $MAP_FILE"

while true; do
    echo ""
    echo "Select palette to edit:"
    for (( i=0; i<PAL_COUNT; i++ )); do
        printf '  %d — %s  (0x%02X-0x%02X)\n' \
            "$i" "${PAL_NAMES[$i]}" "${PAL_START[$i]}" "${PAL_END[$i]}"
    done

    while true; do
        read -r -p "Palette [0-$((PAL_COUNT-1))]: " choice || true
        choice="${choice//$'\r'/}"
        if [[ "$choice" =~ ^[0-9]+$ ]] && (( choice >= 0 && choice < PAL_COUNT )); then break; fi
        echo "  !! Please enter a number between 0 and $((PAL_COUNT-1))"
    done

    edit_palette "$choice"

    read -r -p $'\nEdit another palette? [y/N]: ' again || true
    again="${again//$'\r'/}"
    [[ "${again,,}" == "y" ]] || break
done

save_hex
