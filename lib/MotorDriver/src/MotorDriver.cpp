#include "MotorDriver.h"

MotorDriver::MotorDriver() : in1Pin(-1), in2Pin(-1), enAPin(-1), enBPin(-1), channel(1)
{
}

MotorDriver::MotorDriver(uint8_t in1Pin, uint8_t in2Pin) : in1Pin(in1Pin), in2Pin(in2Pin), enAPin(0), enBPin(0), channel(1)
{
}

MotorDriver::MotorDriver(uint8_t in1Pin, uint8_t in2Pin, uint8_t enAPin, uint8_t enBPin) : in1Pin(in1Pin), in2Pin(in2Pin), enAPin(enAPin), enBPin(enBPin), channel(1)
{
}

bool MotorDriver::init()
{
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    if (enAPin != -1 || enBPin != -1)
    {
        pinMode(enAPin, INPUT);
        pinMode(enBPin, INPUT);
    }
    else
    {
        // log_d("Warning: Hall sensor pins not defined, skipping pinMode setup for enAPin and enBPin.");
    }
    ledcSetup(channel, PWM_FREQUENCY, PWM_BIT_RESOLUTION);
    return true;
}

bool MotorDriver::move(float power, int dir)
{
    if (swapDirection_)
    {
        dir = -dir;
    }

    uint32_t dutyCycle = ((float)MAX_DUTY) * (power / 100);
    if (dir == FORWARD)
    {
        ledcDetachPin(in1Pin);
        ledcAttachPin(in2Pin, channel);
        digitalWrite(in1Pin, LOW);
    }
    else if (dir == REVERSE)
    {
        ledcDetachPin(in2Pin);
        ledcAttachPin(in1Pin, channel);
        digitalWrite(in2Pin, LOW);
    }
    else
    {
        stop();
    }
    ledcWrite(channel, dutyCycle);
    return true;
}

bool MotorDriver::stop()
{
    if (in1Pin == -1)
    {
        return false;
    }
    if (in2Pin == -1)
    {
        return false;
    }
    ledcDetachPin(in1Pin);
    ledcDetachPin(in2Pin);
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    power = 0;
    return true;
}

bool MotorDriver::brake()
{
    if (in1Pin == -1)
    {
        return false;
    }
    if (in2Pin == -1)
    {
        return false;
    }
    ledcDetachPin(in1Pin);
    ledcDetachPin(in2Pin);
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, HIGH);
    power = 0;
    return true;
}

void MotorDriver::setFrequency(uint32_t frequency)
{
    stop();
    ledcSetup(channel, frequency, PWM_BIT_RESOLUTION);
}

void MotorDriver::setSwapDirection(bool swapDirection)
{
    swapDirection_ = swapDirection;
}