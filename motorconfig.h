#include "api.h"
using namespace pros;
//Motor Assignments
//Non Controllor
pros::Controller Controllor1 (CONTROLLER_MASTER);

//Motor
//DriveTrain
Motor FL (6,E_MOTOR_GEARSET_18,true);
Motor BL (7,E_MOTOR_GEARSET_18,false);
Motor BR (8,E_MOTOR_GEARSET_18,false);
Motor FR (9,E_MOTOR_GEARSET_18,true);

//Lift
Motor Rlift (14,E_MOTOR_GEARSET_36,true);
Motor Llift (12,E_MOTOR_GEARSET_36,false);

//Claw
Motor Clawmotor (15,E_MOTOR_GEARSET_36,false);

//3 wire
ADIAnalogIn Pot (D);
