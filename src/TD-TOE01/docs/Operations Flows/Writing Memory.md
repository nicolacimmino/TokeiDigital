

```mermaid
stateDiagram-v2
direction LR

[*] --> NORMAL

NORMAL --> PRG : FN+PRG

PRG --> ADDR : AD
ADDR --> DATA : DA
DATA --> ADDR : AD

PRG --> NORMAL : FN+PRG
ADDR --> NORMAL : FN+PRG
DATA --> NORMAL : FN+PRG
```