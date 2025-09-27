# ESP32 Motor Control Project

This project demonstrates basic DC motor control using ESP-IDF on an ESP32.  
It supports **4 motors (FR, FL, BR, BL)** with PWM speed control and H-bridge direction control.

---

## Table of Motor Pins

| Motor | Control Pin 1 | Control Pin 2 | PWM Pin A | PWM Pin B |
|-------|---------------|---------------|-----------|-----------|
| FR    | 4             | 5             | 19        | 20        |
| FL    | 6             | 7             | 21        | 47        |
| BR    | 16            | 10            | 41        | 42        |
| BL    | 9             | 46            | 2         | 1         |

---

## Features

- Forward and reverse rotation for each motor
- PWM speed control using MCPWM
- Open-drain GPIO support for safe interfacing (if needed)
- Example sequence: forward → speed up → stop → reverse

---

