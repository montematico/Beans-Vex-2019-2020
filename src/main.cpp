/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\1325608                                          */
/*    Created:      Wed Oct 09 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// BLimitSwitch         limit         B               
// TLimitSwitch         limit         C               
// Gyro                 gyro          A               
// FL                   motor         6               
// BL                   motor         7               
// BR                   motor         8               
// FR                   motor         9               
// Rlift                motor         11              
// Llift                motor         12              
// Controller1          controller                    
// IdiotSwitch          bumper        H               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "functions.h"
#include "autofunct.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Startup();
  while(true)
  {
    Gcode();
    //checks if anyof the northturn buttons were pressed
    Ncheck(); 
    DriveTrain(true);
    Lcontrol();
  }
}
