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