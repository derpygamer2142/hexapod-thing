#ifndef LEG_H
#define LEG_H

#include "Vector.hpp"
#include <cmath>
#include <Arduino.h>
#include <ESP32Servo.h>

enum WALKTYPE {
    PLACEHOLDER_SET,
    PLACEHOLDER_WAVE,
    PLACEHOLDER_RIPPLE,
    PLACEHOLDER_TRI
};

/*double constrain(double a, double b, double c) {
    return std::max(b, std::min(a, c));
}*/

double rad(double theta) { return theta * (M_PI/180.0); }

const double coaxiaLength = 3.5;
const double femurLength = 5.0;
const double tibiaLength = 7.5;
const double pushTime = 1.0/3.0; // time the leg is on the ground

int currentId = 0;

class Leg {

    public:

        Servo coaxiaServo = Servo();
        Servo femurServo  = Servo();
        Servo tibiaServo  = Servo();

        /*
         * const double L1 = 88.9;   // Coxa: 3.5"
            const double L2 = 127.0;  // Femur: 5.0"
            const double L3 = 190.5;  // Tibia: 7.5"

         */
        int id;

        const  double ZRest =  2.75591;
        double YRest = 0.0; // this is how much the robot is moved up i think. y is up and you can cry about it 🤫🧏‍♂️

        bool disabled = false;
        
        double progress = 0.0;

        double coxaAngle = 0.0;
        double femurAngle = 0.0;
        double tibiaAngle = 0.0;

        unsigned long lastUpdate = millis();

        double legAngle; // the angle of the leg relative to the forward direction being 0
        vec3 legPosition;

        Leg() : disabled(true) {}

        Leg(int coaxiaPin, int femurPin, int tibiaPin) : id(currentId++) {
            coaxiaServo.attach(coaxiaPin);
            femurServo.attach(femurPin);
            tibiaServo.attach(tibiaPin);
        }

        void initProgress(WALKTYPE type) {
            if (id <= 2) {
                legAngle = -90.0;
            }
            else legAngle = 90.0;

            legAngle += static_cast<double>((id % 3) - 1) * 40.0;
            legPosition = rotate(vec3(0.0, 0.0, 4.0), rad(legAngle));


            switch (type) {
                case (PLACEHOLDER_SET): {
                    if (id == 0 || id == 2 || id == 3 || id == 5) progress = 0.5;
                    else progress = 0.0;
                    break;
                }

                case (PLACEHOLDER_WAVE): {
                    progress = static_cast<double>(id)/12.0;

                    break;
                }

                case (PLACEHOLDER_RIPPLE): {
                    progress = 0.5-(fabs(static_cast<double>(id) - 2.5)/5.0);

                    break;
                }

                case (PLACEHOLDER_TRI): {
                    if (id == 0 || id == 2 || id == 5) progress = 0.5;
                    else progress = 0.0;

                    break;
                }
            }
        }

        vec3 findJointAngles(vec3 point) {
            double X = point.x(), 
                   Z = point.y(),  // this equation is z-up, the point given is y-up
                   Y = point.z();
            Z += YRest;
            Y += ZRest;

            // CALCULATE INVERSE KINEMATIC SOLUTION
            double J1 = atan(X / Y) * (180 / M_PI);
            double H = sqrt((Y * Y) + (X * X));
            double L = sqrt((H * H) + (Z * Z));
            double J3 = acos(   ((femurLength * femurLength) + (tibiaLength * tibiaLength) - (L * L))   /   (2 * femurLength * tibiaLength)   ) * (180 / M_PI);
            double B = acos(   ((L * L) + (femurLength * femurLength) - (tibiaLength * tibiaLength))   /   (2 * L * femurLength)   ) * (180 / M_PI);
            double A = atan(Z / H) * (180 / M_PI);  // BECAUSE Z REST IS NEGATIVE, THIS RETURNS A NEGATIVE VALUE
            double J2 = (B + A);  // BECAUSE 'A' IS NEGATIVE AT REST WE NEED TO INVERT '-' TO '+'

            return vec3(J1, J2, J3);
        }

        vec3 getCurrentTargetPoint(vec3 targetPosition) {
            double yPos = 0.0;
            if (progress >= pushTime) {
                yPos = 1.0 - 2*fabs(progress-(pushTime+0.5*(1-pushTime)))/(1.0-pushTime); // sigma equation to mix the time the leg is on the ground and the time in the air
            }
            
            double angle = 45.0; // the angle the end of the leg should be placed at relative to the fixation point

            double offset = progress < pushTime ?
                progress/pushTime :
                1.0 - (progress-pushTime)/(1-progress);

            angle += 50.0*offset;

            vec3 point = rotate(vec3(0.0, -4.0, 1.0/distance(legPosition, targetPosition)), angle); // the position of the end of the leg relative to where the leg is fixed

            return point + legPosition;
        }

        void updateProgress() {
            // todo: implement controller movement into this
            double elapsed = static_cast<double>(millis() - lastUpdate);
            lastUpdate = millis();



            progress += elapsed/200.0; // this should only go up by ~.0025 each tick
        }

        void updateLeg(vec3 targetPosition) {
            if (!disabled) {
                updateProgress();

                vec3 angles = findJointAngles(getCurrentTargetPoint(targetPosition));

                coaxiaServo.write(static_cast<int>(angles.x()));
                femurServo.write(static_cast<int>(angles.y()));
                tibiaServo.write(static_cast<int>(angles.z()));
            }
        }
};



#endif