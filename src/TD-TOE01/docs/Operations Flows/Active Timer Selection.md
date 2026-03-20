```mermaid
stateDiagram-v2
direction LR

[*] --> T1

T1 --> T2 : SEL
T2 --> T3 : SEL
T3 --> TIME : SEL
TIME --> T1 : SEL

state T1
state T2
state T3
state TIME
```