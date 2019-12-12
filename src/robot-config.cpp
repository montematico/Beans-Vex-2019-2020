#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
limit BLimitSwitch = limit(Brain.ThreeWirePort.B);
limit TLimitSwitch = limit(Brain.ThreeWirePort.C);
gyro Gyro = gyro(Brain.ThreeWirePort.A);
motor FL = motor(PORT6, ratio18_1, true);
motor BL = motor(PORT7, ratio18_1, false);
motor BR = motor(PORT8, ratio18_1, false);
motor FR = motor(PORT9, ratio18_1, true);
motor Rlift = motor(PORT14, ratio36_1, true);
motor Llift = motor(PORT12, ratio36_1, false);
controller Controller1 = controller(primary);
motor Clawmotor = motor(PORT13, ratio36_1, false);
pot Pot = pot(Brain.ThreeWirePort.D);
encoder Xencode = encoder(Brain.ThreeWirePort.E);
encoder Yencode = encoder(Brain.ThreeWirePort.G);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}