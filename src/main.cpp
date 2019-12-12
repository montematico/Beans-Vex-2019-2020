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
// Rlift                motor         14              
// Llift                motor         12              
// Controller1          controller                    
// Clawmotor            motor         13              
// Pot                  pot           D               
// Xencode              encoder       E, F            
// Yencode              encoder       G, H            
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// Rlift                motor         14              
// Llift                motor         12              
// Controller1          controller                    
// Clawmotor            motor         13              
// Pot                  pot           D               
// Xencode              encoder       E, F            
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// Rlift                motor         14              
// Llift                motor         12              
// Controller1          controller                    
// Clawmotor            motor         13              
// Pot                  pot           D               
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// Rlift                motor         14
// Llift                motor         12
// Controller1          controller
// Clawmotor            motor         13
// Pot                  pot           D
// Xencode              encoder       E, F
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// Rlift                motor         14
// Llift                motor         12
// Controller1          controller
// Clawmotor            motor         13
// Pot                  pot           D
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "vex.h"
#include "autofunct.h"
#include "functions.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  motorset();
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Add gui autocode selector
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
extern double dist[2];
void autonomous(void) {
  std::cout << "Autonon start" << std::endl;
  Startup();
  std::cout << "Gyro Calibrated" << std::endl;

  //int blue = -1; //set to -1 for blue, 1 for red.
  Pgo(10,12);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  motorset(); //this is incase pre auton doenst work or we're driving it for practice.
  // User control code here, inside the loop
  task DriveTrain = task(DriveTrainCallback); //Creates Instance of drivetrain.
  DriveTrain.setPriority(25);
  while (1) {
    Gcode(); //Displays cool things on screen.
    Ncheck(); //Checks if northturn buttons are pressed
    //Runs drivetrain, bool was for a depreciated function.
    Lcontrol(); //Controls the lift
    ClawControl(); //controls the claw.

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//

int main() {

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
