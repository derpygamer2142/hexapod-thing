#include <Arduino.h>

#include "Leg.hpp"
#include "Controller.hpp"
#include <Bluepad32.h>
#include <optional>

double lift = 0;
const double maxLift = 15.748;



Leg* legs[6];

vec3 controllerTargetPosition = vec3(0.0);

void processGamepad(ControllerPtr ctl) {
    if (ctl->throttle() > 10){
        if (lift < maxLift) {
            lift += 0.19685;

            for (int i = 0; i < 6; i++) {
                legs[i]->YRest = lift;
            }
        }
    }

    controllerTargetPosition = 4.0*normalize(vec3(static_cast<double>(ctl->axisX())/512.0, 0.0, static_cast<double>(ctl->axisY())/512.0)); // target position of the controller with length 4

    
    dumpGamepad(ctl);
 
}

void setup() {
    Serial.begin(115200);
    legs[0] = new Leg(18,19,21);
    for (int i = 1; i < 6; i++) {
        legs[i] = new Leg();
    }
    

    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", bdAddr[0], bdAddr[1], bdAddr[2], bdAddr[3], bdAddr[4], bdAddr[5]);
}

void loop() {
    // put your main code here, to run repeatedly:
    updateController(processGamepad);
    
    for (int i = 0; i < 6; i++) {
        legs[i]->updateLeg(controllerTargetPosition);
    }

    

    delay(5);
}
