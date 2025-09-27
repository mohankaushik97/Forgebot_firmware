#ifndef PICCONTROLLER_H
#define PICCONTROLLER_H

#include <Arduino.h>

#include <memory>

#include "settings.h"

#include "MotorDriver.h"
#include "MotorEncoder.h"

class PIDController
{

public:
    PIDController(std::shared_ptr<MotorDriver> d, std::shared_ptr<MotorEncoder> e, float targetVel);

    void calculateVelocity();

    void velController();

    void setTargetVelocity(float targetVel);

    void setTimer();

    void IRAM_ATTR onTimer();

    static void IRAM_ATTR onTimerTrampoline(void* arg);

private:
    std::shared_ptr<MotorDriver> driver;
    std::shared_ptr<MotorEncoder> encoder;
    float targetVelocity;
    hw_timer_t * timer;
    portMUX_TYPE timerMux;

    int32_t currPos, prevPos;

};

#endif