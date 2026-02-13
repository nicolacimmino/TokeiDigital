#!/usr/bin/env bash
# TD-FDCF01A LED Color HEX file editor — multi-palette

set -e

INPUT="dcf77.eeprom.hex"

if [[ ! -f "$INPUT" ]]; then
    echo "Error: file not found: $INPUT"
    exit 1
fi

# Palette base addresses (each palette spans 0x30 bytes)
PALETTE_BASE=(0x40 0x70 0xA0 0xD0)

# ---------------------------------------------------------------
# Parse Intel HEX file into MEM[0..255]
# ---------------------------------------------------------------
declare -a MEM
for (( i=0; i<256; i++ )); do MEM[$i]=0; done

parse_hex() {
    while IFS= read -r line; do
        [[ "$line" =~ ^:([0-9A-Fa-f]{2})([0-9A-Fa-f]{4})00([0-9A-Fa-f]+) ]] || continue
        local byte_count=$(( 16#${BASH_REMATCH[1]} ))
        local addr=$(( 16#${BASH_REMATCH[2]} ))
        local data="${BASH_REMATCH[3]}"
        data="${data:0:$(( byte_count * 2 ))}"
        for (( i=0; i<byte_count; i++ )); do
            local b=$(( 16#${data:$(( i*2 )):2} ))
            local idx=$(( addr + i ))
            if (( idx < 256 )); then MEM[$idx]=$b; fi
        done
    done < "$1"
}

parse_hex "$INPUT"

# ---------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------
read_rgb() {
    printf '%02X%02X%02X' "${MEM[$1]}" "${MEM[$(( $1+1 ))]}" "${MEM[$(( $1+2 ))]}"
}

write_rgb() {
    local addr=$1 hex="$2"
    MEM[$addr]=$(( 16#${hex:0:2} ))
    MEM[$(( addr+1 ))]=$(( 16#${hex:2:2} ))
    MEM[$(( addr+2 ))]=$(( 16#${hex:4:2} ))
}

ask_color() {
    local label="$1" addr=$2 current val
    current=$(read_rgb "$addr")
    while true; do
        read -r -p "$label  [current: $current]: " val
        val="${val:-$current}"; val="${val^^}"; val="${val#0X}"
        if [[ "$val" =~ ^[0-9A-F]{6}$ ]]; then
            write_rgb "$addr" "$val"
            return
        fi
        echo "  !! Enter exactly 6 hex digits (e.g. FF0000)"
    done
}

# ---------------------------------------------------------------
# Edit one palette given its base address
# ---------------------------------------------------------------
edit_palette() {
    local base=$1
    local b=$(( base ))
    printf '\nEditing palette — base address: 0x%02X\n' "$b"
    echo "Enter 6 hex digits (RRGGBB) or press ENTER to keep current value."
    ask_color "OFF LEDs color          ($(printf '0x%02X-0x%02X' $((b+0x00)) $((b+0x02))))" $(( b+0x00 ))
    ask_color "Minutes LEDs color      ($(printf '0x%02X-0x%02X' $((b+0x03)) $((b+0x05))))" $(( b+0x03 ))
    ask_color "Hours LEDs color        ($(printf '0x%02X-0x%02X' $((b+0x06)) $((b+0x08))))" $(( b+0x06 ))
    ask_color "Day-of-Month LEDs color ($(printf '0x%02X-0x%02X' $((b+0x09)) $((b+0x0B))))" $(( b+0x09 ))
    ask_color "Day-of-Week LEDs color  ($(printf '0x%02X-0x%02X' $((b+0x0C)) $((b+0x0E))))" $(( b+0x0C ))
    ask_color "Month LEDs color        ($(printf '0x%02X-0x%02X' $((b+0x0F)) $((b+0x11))))" $(( b+0x0F ))
    ask_color "Year LEDs color         ($(printf '0x%02X-0x%02X' $((b+0x12)) $((b+0x14))))" $(( b+0x12 ))
    ask_color "Flags LEDs color        ($(printf '0x%02X-0x%02X' $((b+0x15)) $((b+0x17))))" $(( b+0x15 ))
    ask_color "Markers LEDs color      ($(printf '0x%02X-0x%02X' $((b+0x18)) $((b+0x1A))))" $(( b+0x18 ))
    ask_color "Civil Warning LEDs color($(printf '0x%02X-0x%02X' $((b+0x1B)) $((b+0x1D))))" $(( b+0x1B ))
    ask_color "Seconds Hand LEDs color ($(printf '0x%02X-0x%02X' $((b+0x1E)) $((b+0x20))))" $(( b+0x1E ))
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
    local bytes=("$@") byte_count=32
    local cs_args=( "$byte_count" $(( (addr>>8)&0xFF )) $(( addr&0xFF )) 0 "${bytes[@]}" )
    local cs; cs=$(checksum "${cs_args[@]}")
    printf ':%02X%04X00' "$byte_count" "$addr"
    for b in "${bytes[@]}"; do printf '%02X' "$b"; done
    printf '%s\n' "$cs"
}

save_hex() {
    # Backup first
    cp "$INPUT" "${INPUT}.bak"
    # Write updated file
    {
        for (( row=0; row<8; row++ )); do
            local base=$(( row * 32 )) row_bytes=()
            for (( col=0; col<32; col++ )); do
                row_bytes+=( "${MEM[$(( base + col ))]}" )
            done
            hex_record "$base" "${row_bytes[@]}"
        done
        echo ":00000001FF"
    } > "$INPUT"
    echo "Backup saved to: ${INPUT}.bak"
    echo "File saved to:   $INPUT"
}

# ---------------------------------------------------------------
# Main loop
# ---------------------------------------------------------------
echo "TD-FDCF01A LED Color Editor — file: $INPUT"

while true; do
    echo ""
    echo "Select palette to edit:"
    echo "  0 — Palette 0  (0x40-0x6F)"
    echo "  1 — Palette 1  (0x70-0x9F)"
    echo "  2 — Palette 2  (0xA0-0xCF)"
    echo "  3 — Palette 3  (0xD0-0xFF)"

    while true; do
        read -r -p "Palette [0-3]: " choice
        if [[ "$choice" =~ ^[0-3]$ ]]; then break; fi
        echo "  !! Please enter 0, 1, 2 or 3"
    done

    edit_palette "${PALETTE_BASE[$choice]}"

    read -r -p $'\nEdit another palette? [y/N]: ' again
    [[ "${again,,}" == "y" ]] || break
done

save_hex