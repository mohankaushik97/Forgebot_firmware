#include <Arduino.h>
#include "settings.h"

#ifndef MOTORENCODER_H
#define MOTORENCODER_H

using namespace std;
class MotorEncoder
{
public:
    MotorEncoder(uint8_t pinA, uint8_t pinB);

    void init();
    void measurePos();

    void setSwapDirection(bool swapDirection_);
    static void measurePosHandler();

    tuple<bool, float, int> motorControl(int32_t target);

    int32_t getPos();
    void printPos();
    int8_t getEnA();


private:
    int8_t dir;
    int32_t pos;
    int32_t targetPos;
    float power;
    uint32_t currT;
    uint32_t prevT;
    float deltaT;
    float e;
    float ePrev;
    float eIntegral;
    float eDt;
    float u; // Control Signal
    int i;

    uint8_t enA;
    uint8_t enB;
    bool swapDirection;

    static MotorEncoder *instance;
};
#endif