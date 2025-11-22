# Controls Leader

This board is the central hub for human machine interface inputs on the vehicle. It reads switch states, detects faults, and coordinates lighting commands across the vehicle network while maintaining robust error reporting.

## Hardware Architecture

### Power Delivery

- Main 24 V supply with ground input
- Buck regulator for MCU and Peripheral Sensing/Output Modules (PSOMs)
- Isolated power and ground dedicated to CAN interface(s)

### CAN Interfaces

- **2× CAN Transceiver Chips** with dedicated in/out connector pairs:
  - Controls CAN (lighting/peripheral network)
  - Car CAN (vehicle state network)

### Input Interfaces

#### Dashboard I/O

- Ignition switch
- Gear selector switch
- Cruise control enable/set buttons
- Regen enable button
- Hazard lights switch
- Supplemental battery switch (isolated input)

#### Steering Wheel I/O

- Left/right turn indicators
- Horn button
- Regen active indicator

## Software Functionality
TBD

## Repository Structure

```
hardware/
├── ControlsLeader_PCB/
│   ├── jkcpcb/
│   ├── lib/
│   ├── *.kicad_sch (schematic files)
│   └── *.kicad_pcb (PCB layout)
├── 3d_models/
├── Silkscreens/
└── fab/

firmware/

```

