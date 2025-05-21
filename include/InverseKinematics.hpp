#ifndef IK_H
#define IK_H

#include "Vector.hpp"

enum WALKTYPE {
    PLACEHOLDER_SET,
    PLACEHOLDER_WAVE,
    PLACEHOLDER_RIPPLE,
    PLACEHOLDER_TRI
};

class Leg {

    public:
        double femurLength;
        double tibiaLength;
        int id;
        static inline int currentId = 0;
        
        double progress = 0.0;

        double coxaAngle = 0.0;
        double femurAngle = 0.0;
        double tibiaAngle = 0.0;

        Leg(double fl, double tl) : femurLength(fl), tibiaLength(tl), id(currentId++) {}

        void initProgress(WALKTYPE type) {
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
                    progress = 0.5-(abs(static_cast<double>(id) - 2.5)/5.0);

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
            
        }
};


#endif