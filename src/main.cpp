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
// Rlift                motor         11              
// Llift                motor         12              
// Controller1          controller                    
// IdiotSwitch          bumper        H               
// Clawmotor            motor         13              
// Pot                  pot           D               
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
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
// Clawmotor            motor         13              
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
  //Add gui autocode selector
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

void autonomous(void) {
  Startup();

  //Yanks lift up and done to deploy claw.
  DLcontrol(100); //This couldn't be its own functions because it calls DLcontrol which is in a seperate file than autofunct.h
  wait(0.3, sec);
  DLcontrol(-80);
  wait(0.2, sec);
  std::cout<<"Claw Deployed"<<std::endl;
  Brain.Screen.printAt(1, 60, "Claw Deployed");
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
  
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.


    Gcode();
    //checks if anyof the northturn buttons were pressed
    Ncheck(); 
    DriveTrain(true);
    Lcontrol();
    ClawControl();
    std::cout<<Pot.angle(rotationUnits::deg)<<std::endl;
    std::cout<<Clawmotor.isSpinning()<<std::endl;
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
                    
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() 
{
  
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
