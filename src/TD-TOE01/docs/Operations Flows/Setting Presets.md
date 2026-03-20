```mermaid
stateDiagram-v2
direction LR

[*] --> READY

state READY : time set
state PRE : PRE mode
state STORE : hold 0-9
state CONFIRM : flash preset #
state EXIT : PRE off

READY --> PRE : FN+PRE

PRE --> STORE : hold 0-9

STORE --> CONFIRM : stored

CONFIRM --> EXIT : 2s

EXIT --> READY
```