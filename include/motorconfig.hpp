#include "main.h"
using namespace pros;


//Controller
pros::Controller controller (E_CONTROLLER_MASTER);

//DriveTrain
pros::Motor FL (6,E_MOTOR_GEARSET_18,true);
pros::Motor BL (7,E_MOTOR_GEARSET_18,false);
pros::Motor BR (8,E_MOTOR_GEARSET_18,false);
pros::Motor FR (9,E_MOTOR_GEARSET_18,true);

//Lift
pros::Motor Rlift (14,E_MOTOR_GEARSET_36,true);
pros::Motor Llift (12,E_MOTOR_GEARSET_36,false);

//Claw
pros::Motor Clawmotor (16,E_MOTOR_GEARSET_36,false);

//3 wire

pros::ADIGyro Gyro (1,0.1);
pros::ADIPotentiometer Pot (4);
pros::ADIEncoder Xencode(5,6);
pros::ADIEncoder Yencode(7,8);
