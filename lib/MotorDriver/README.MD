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