#include "MotorEncoder.h"

MotorEncoder *MotorEncoder::instance = nullptr;

MotorEncoder::MotorEncoder(uint8_t pinA, uint8_t pinB) : enA(pinA), enB(pinB), swapDirection(false)
{
}

void MotorEncoder::init()
{
    if (swapDirection)
    {
        uint8_t temp = enA;
        enA = enB;
        enB = temp;
    }
    pinMode(enA, INPUT);
    pinMode(enB, INPUT);

    instance = this;
    i = 0;

    // attachInterrupt(enA, MotorEncoder::measurePosHandler, RISING);
}

// void MotorEncoder::measurePosHandler()
// {
//     if (instance != nullptr)
//     {
//         // Serial.println("Reaching this point because of instance!!");
//         instance->measurePos();
//     }
//     else
//     {
//         // Serial.println("No instance!!!");
//     }
// }

int8_t MotorEncoder::getEnA()
{
    return enA;
}

void MotorEncoder::measurePos()
{
    int enb = digitalRead(enB);
    if (enb == HIGH)
    {
        pos--;
    }
    else
    {
        pos++;
    }
}

void MotorEncoder::setSwapDirection(bool swapDirection_)
{
    swapDirection = swapDirection_;
    init();
}

tuple<bool, float, int> MotorEncoder::motorControl(int32_t target)
{
    targetPos = target;

    currT = micros();
    deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;

    // if (swapDirection)
    // {
    //     e = pos - targetPos;
    // }
    e = targetPos - pos;
    eIntegral = eIntegral + e * deltaT;
    eDt = (e - ePrev) / (deltaT);
    ePrev = e;

    u = Kp * e + Ki * eIntegral + Kd * eDt;

    power = fabs(u);
    if (power > MAX_DUTY)
    {
        power = MAX_DUTY;
    }

    if (u < 0)
    {
        dir = REVERSE;
    }
    else
    {
        dir = FORWARD;
    }
    bool reached;
    if (e < TOLERANCE)
    {
        reached = true;
    }
    reached = false;
    return make_tuple(reached, power, dir);
}

int32_t MotorEncoder::getPos()
{
    return pos;
}

void MotorEncoder::printPos()
{
    if (i == 100)
    {
        Serial.print(dir);
        Serial.print(", ");
        Serial.print(power);
        Serial.print(", ");
        Serial.print(targetPos);
        Serial.print(", ");
        Serial.println(pos);
        i = 0;
    }
    else
    {
        i++;
    }
}