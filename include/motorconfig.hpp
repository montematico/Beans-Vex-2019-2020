#include "main.h"
using namespace pros;


//Controller
pros::Controller controller (E_CONTROLLER_MASTER);

//DriveTrain
pros::Motor FL (6,E_MOTOR_GEARSET_18,false);
pros::Motor BL (7,E_MOTOR_GEARSET_18,false);
pros::Motor BR (8,E_MOTOR_GEARSET_18,false);
pros::Motor FR (9,E_MOTOR_GEARSET_18,false);

//Lift
pros::Motor Rlift (20,E_MOTOR_GEARSET_36,true);
pros::Motor Llift (1,E_MOTOR_GEARSET_36,false);

//Claw
pros::Motor Clawmotor (19,E_MOTOR_GEARSET_36,false);

//3 wire
pros::ADIDigitalIn Ulimit (7);
pros::ADIDigitalIn Dlimit (8);
pros::ADIUltrasonic Rsense(3,4);
pros::ADIUltrasonic Lsense(5,6);
pros::ADIUltrasonic Cubesense('A','B');

//Vision
pros::Vision vision_sensor (18,E_VISION_ZERO_TOPLEFT);
