#include "PIDController.h"

PIDController::PIDController(std::shared_ptr<MotorDriver> d, std::shared_ptr<MotorEncoder> e, float targetVel) : driver(d), encoder(e), targetVelocity(targetVel){

}

void PIDController::setTimer(){
    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer, &PIDController::onTimerTrampoline, false);
    timerAlarmWrite(timer,1000,true);
    timerAlarmEnable(timer);
    
}

void IRAM_ATTR PIDController::onTimer(){
    currPos = encoder->getPos();
}

void IRAM_ATTR PIDController::onTimerTrampoline(void* arg){
    PIDController* controller = static_cast<PIDController*>(arg);
    controller->onTimer();
}