#include <Arduino.h>
#include "settings.h"
#ifndef DRV8833_H
#define DRV8833_H

class MotorDriver
{
public:
    MotorDriver();
    MotorDriver(uint8_t in1Pin, uint8_t in2Pin);
    MotorDriver(uint8_t in1Pin, uint8_t in2Pin, uint8_t enAPin, uint8_t enBPin);

    // Initialize the DRV8833 object. Default decay mode: SLOW
    // input:
    //   in1Pin       : DRV8833 AIN1/BIN1
    //   in2Pin       : DRV8833 AIN2/BIN2
    //   enAPin       : Pin A for halls sensor
    //   enBPin       : Pin B for halls sensor
    // return:
    //   true if no error occurs.
    bool init();

    // Set motor rotation direction/speed
    // input:
    //   power: rotation power. -100..100
    //          Negative numbers mean reverse rotation; positive numbers mean forward direction.
    //          Zero stop the rotation
    // return:
    //   true if no error occurs.
    bool move(float power, int dir); // use fast decay -> smooth movement

    // Stop the motor, using fast decay mode
    // return:
    //   true if no error occurs.
    bool stop();

    // Stop the motor, using slow decay mode
    // return:
    //   true if no error occurs.
    bool brake();

    // Set the PWM frequency (for ESP32 ledc functions)
    // input:
    //   frequency: new frequency in Hertz. 1..50000Hz.
    void setFrequency(uint32_t frequency);

    // Set the channel for PWM use.
    // input:
    // pwm channels in esp32
    void setChannel(uint8_t channel_);

    // Swap the motor rotation direction.
    // Useful for wrong cabling / driving two opposite motors (left wheel + right wheel motor of a vehicle)
    // input:
    //   swapDirection: true -> swap rotation direction
    void setSwapDirection(bool swapDirection);

private:
    int8_t in1Pin;
    int8_t in2Pin;
    int8_t enAPin;
    int8_t enBPin;
    uint8_t channel;
    int8_t power;
    bool swapDirection_;
};

#endif