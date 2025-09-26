# Encoder & Motor Wiring Guide

## Encoder Readings

- **Clockwise Rotation:** `S1 → S2`  
- **Anti-Clockwise Rotation:** `S2 → S1`

**Logic Table:**

| Event           | Condition       | Direction     |
|-----------------|-----------------|---------------|
| `S1` Triggered  | `S2 = LOW`      | Clockwise     |
| `S1` Triggered  | `S2 = HIGH`     | Anti-Clockwise|

---

## Motor Wiring

| Motor Wire | Connection | Note |
|------------|------------|------|
| **Red (M-)**  | OUT1       |      |
| **Black (M+)**| OUT2       |      |

**Direction Control:**

| OUT1 | OUT2 | Direction  |
|------|------|-------------|
| LOW  | HIGH | Clockwise   |

---

## Encoder Pinout

| Encoder Wire | Connection | Function |
|--------------|------------|----------|
| **White**    | VCC        | Power    |
| **Yellow**   | GND        | Ground   |
| **Orange**   | S1 (ENCA)  | Channel A|
| **Green**    | S2 (ENCB)  | Channel B|

---

## Library capabilities

| Library          | Level              | Function                              |
|------------------|--------------------|---------------------------------------|
| **MotorDriver**  | Single Motor       | Control — Speed, Direction            |
| **MotorEncoder** | Single Motor       | Measure — Speed, Direction            |
| **PIDController**| Single Motor       | Closed-loop — Position, Velocity      |
| **Kinematics**   | Motor Integration  | Robot — Position, Velocity            |
| **Communication**| micro-ROS Integration | Input — Velocity, Output — Position |

---
---

# MotorDriver Class - Function Reference

| Function | Parameters | Description | Returns |
|----------|------------|-------------|---------|
| `MotorDriver()` | — | Default constructor | — |
| `MotorDriver(uint8_t in1Pin, uint8_t in2Pin)` | `in1Pin`, `in2Pin` | Constructor with motor pins | — |
| `MotorDriver(uint8_t in1Pin, uint8_t in2Pin, uint8_t enAPin, uint8_t enBPin)` | `in1Pin`, `in2Pin`, `enAPin`, `enBPin` | Constructor with motor and encoder pins | — |
| `bool init()` | — | Initialize the DRV8833 motor driver. Default decay mode: SLOW. | `true` if no error occurs |
| `bool move(float power, int dir)` | `power` (-100..100), `dir` | Set motor rotation speed and direction. Negative = reverse, positive = forward, zero = stop. Uses fast decay for smooth movement. | `true` if no error occurs |
| `bool stop()` | — | Stop the motor using fast decay mode | `true` if no error occurs |
| `bool brake()` | — | Stop the motor using slow decay mode | `true` if no error occurs |
| `void setFrequency(uint32_t frequency)` | `frequency` (1..50000 Hz) | Set PWM frequency for ESP32 ledc | — |
| `void setChannel(uint8_t channel_)` | `channel_` | Set the PWM channel | — |
| `void setSwapDirection(bool swapDirection)` | `swapDirection` | Swap motor rotation direction (useful for correcting cabling or controlling opposite wheels) | — |

---

# MotorEncoder Class - Function Reference

| Function | Parameters | Description | Returns |
|----------|------------|-------------|---------|
| `MotorEncoder(uint8_t pinA, uint8_t pinB)` | `pinA`, `pinB` | Constructor. Assigns encoder pins A and B | — |
| `void init()` | — | Initialize the encoder | — |
| `void measurePos()` | — | Measure and update the current position of the motor | — |
| `void setSwapDirection(bool swapDirection_)` | `swapDirection_` | Swap the encoder reading direction | — |
| `static void measurePosHandler()` | — | Static interrupt handler for position measurement | — |
| `tuple<bool, float, int> motorControl(int32_t target)` | `target` | Compute control signal to reach target position | Tuple: `(success, power, direction)` |
| `int32_t getPos()` | — | Get the current position of the motor | `pos` |
| `void printPos()` | — | Print the current position to Serial | — |
| `int8_t getEnA()` | — | Get the encoder pin A number | `enA` |

---

# Multi-Motor Robot Code Structure with PID (PlatformIO)

This document describes a recommended **PlatformIO project structure** for building a multi-motor robot with PID control. It separates hardware abstraction, control logic, and application orchestration for better scalability and maintainability.

---

## 📂 Suggested Project Structure

```
├── include/
│   ├── config.h              # Pin mappings, constants
│   └── types.h               # Common typedefs/structs
│
├── lib/
│   ├── MotorDriver/
│   │   ├── src/
│   │   │   ├── MotorDriver.cpp
│   │   │   └── MotorDriver.h
│   │   └── library.json      # Optional metadata
│   │
│   ├── MotorEncoder/
│   │   ├── src/
│   │   │   ├── MotorEncoder.cpp
│   │   │   └── MotorEncoder.h
│   │   └── library.json
│   │
│   ├── PIDController/
│   │   ├── src/
│   │   │   ├── PID.cpp
│   │   │   └── PID.h
│   │   └── library.json
│   │
│   ├── Kinematics/
│   │   ├── src/
│   │   │   ├── Kinematics.cpp    # e.g. differential / omni calculations
│   │   │   └── Kinematics.h
│   │   └── library.json
│   │
│   └── Communication/
│       ├── src/
│       │   ├── Comms.cpp         # Serial, ROS, Wi-Fi comms
│       │   └── Comms.h
│       └── library.json
│
├── src/
│   └── main.cpp               # Setup + main control loop
│
├── platformio.ini
└── README.md
```

---

## 📌 Responsibilities of Each Module

- **`MotorDriver`**: Low-level control of motor pins (PWM, direction).  
- **`MotorEncoder`**: Encoder tick counting, velocity, position.  
- **`PIDController`**: Generic PID implementation (can be reused for velocity, position, heading).  
- **`Kinematics`**: Translates robot-level commands (vx, vy, ω) into per-wheel targets. Different drives (differential, omni, mecanum) live here.  
- **`Communication`**: Handles commands from RPi/ROS or serial console. Converts high-level velocity commands into setpoints for PID.  
- **`main.cpp`**: Only orchestrates:
  1. Init modules  
  2. Read sensors  
  3. Run control (PID, kinematics)  
  4. Send motor commands  
  5. Publish telemetry  

---

## 📄 Example `main.cpp` (skeleton)

```cpp
#include <Arduino.h>
#include "config.h"

#include <MotorDriver.h>
#include <MotorEncoder.h>
#include <PID.h>
#include <Kinematics.h>
#include <Comms.h>

// Four motors (omni or mecanum example)
MotorDriver motorFL(FL1, FL2, FL_PWM);
MotorDriver motorFR(FR1, FR2, FR_PWM);
MotorDriver motorBL(BL1, BL2, BL_PWM);
MotorDriver motorBR(BR1, BR2, BR_PWM);

MotorEncoder encFL(FL_A, FL_B);
MotorEncoder encFR(FR_A, FR_B);
MotorEncoder encBL(BL_A, BL_B);
MotorEncoder encBR(BR_A, BR_B);

PID pidFL(1.0, 0.01, 0.1); // Tune values
PID pidFR(1.0, 0.01, 0.1);
PID pidBL(1.0, 0.01, 0.1);
PID pidBR(1.0, 0.01, 0.1);

Kinematics kinematics(WHEEL_RADIUS, WHEEL_BASE, TRACK_WIDTH);
Comms comms;

void setup() {
    Serial.begin(115200);
    motorFL.init();
    motorFR.init();
    motorBL.init();
    motorBR.init();

    encFL.init();
    encFR.init();
    encBL.init();
    encBR.init();

    comms.init();
}

void loop() {
    // 1. Read encoders
    float velFL = encFL.getVelocity();
    float velFR = encFR.getVelocity();
    float velBL = encBL.getVelocity();
    float velBR = encBR.getVelocity();

    // 2. Get high-level command (e.g. from ROS: vx, vy, omega)
    RobotCommand cmd = comms.getCommand();

    // 3. Convert to wheel targets
    WheelVelocities targets = kinematics.inverse(cmd.vx, cmd.vy, cmd.omega);

    // 4. PID control per wheel
    float outFL = pidFL.compute(targets.fl, velFL);
    float outFR = pidFR.compute(targets.fr, velFR);
    float outBL = pidBL.compute(targets.bl, velBL);
    float outBR = pidBR.compute(targets.br, velBR);

    // 5. Send to motors
    motorFL.setPWM(outFL);
    motorFR.setPWM(outFR);
    motorBL.setPWM(outBL);
    motorBR.setPWM(outBR);

    // 6. Telemetry back
    comms.publishOdom(encFL.getPos(), encFR.getPos(), encBL.getPos(), encBR.getPos());
}
```

---

✅ Benefits of this structure:
- **Scalable**: Add more drives (arms, turrets) without cluttering `main.cpp`.  
- **Reusable**: `PID` and `Kinematics` are generic.  
- **Testable**: Each module can be tested in isolation.  
- **Industry-like**: Mirrors ROS layering (drivers → control → kinematics → comms).  

---
