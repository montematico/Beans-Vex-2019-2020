#include "vex.h"

// Functions makes the code readable.
// Making Drive Train Variables Global
int FLI;
int BLI;
int FRI;
int BRI;
// More Variables for gnocchi code
int Goff = 0; // Gyroscope offset, set to zero because its non functional

void DriveTrain(bool run) // Contains code for driving motors
{
  // All values that need to be calculated for the x-drive
  // https://www.youtube.com/watch?v=1-Ju_VqYLAU
  // Todo Read Gyro and offset "forward" for Field Centric Drive
  // If you really hate yourself add more Gyro's and average values to clean up
  // errors If you truly hate yourself use linesensors to check when you go over
  // the line and reset the gyro to keep it accurate
 if(run)
 {
    FLI = Controller1.Axis3.position(vex::percentUnits::pct) +
        (1 * Controller1.Axis4.position(vex::percentUnits::pct)) +
        Controller1.Axis1.position(vex::percentUnits::pct) - Goff;
    BLI = Controller1.Axis3.position(vex::percentUnits::pct) -
        (1 * Controller1.Axis4.position(vex::percentUnits::pct)) +
        Controller1.Axis1.position(vex::percentUnits::pct) - Goff;
    FRI = (-1 * Controller1.Axis3.position(vex::percentUnits::pct)) +
        (1 * Controller1.Axis4.position(vex::percentUnits::pct)) +
        Controller1.Axis1.position(vex::percentUnits::pct) - Goff;
    BRI = (-1 * Controller1.Axis3.position(vex::percentUnits::pct)) -
        (1 * Controller1.Axis4.position(vex::percentUnits::pct)) +
        Controller1.Axis1.position(vex::percentUnits::pct) - Goff;

  FL.spin(vex::directionType::fwd, FLI, vex::velocityUnits::pct);
  BL.spin(vex::directionType::fwd, BLI, vex::velocityUnits::pct);
  FR.spin(vex::directionType::fwd, FRI, vex::velocityUnits::pct);
  BR.spin(vex::directionType::fwd, BRI, vex::velocityUnits::pct);
 } else {} //blank because break command was acting up

}

void NorthTurn(int dir)
{
  int pwr = 50; //turn rate 0-100 %
  
  while ( fabs(dir - Gyro.value(vex::rotationUnits::deg)) >= 3) 
  {

    FL.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
    FR.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
    BL.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
    BR.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
  }
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
}
void Ncheck()
{
  if(Controller1.ButtonUp.pressing()) {
    NorthTurn(0);} 
  else if(Controller1.ButtonRight.pressing()){ 
    NorthTurn(270);}
  else if(Controller1.ButtonLeft.pressing()){
    NorthTurn(90);}
  else if(Controller1.ButtonDown.pressing()){ 
    NorthTurn(180);}
  else if(Controller1.ButtonY.pressing())
    {
      Gyro.setHeading(0,degrees); //Sets new "north" if Y button is pressed
    }
}

void DLcontrol(int pwr) {
  // Synchronizes motors for lift
  // uses pwr as a -100-100 integer for controlling both lift motors
  if (pwr != 0) {
    Llift.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
    Rlift.spin(vex::directionType::fwd, pwr, vex::velocityUnits::rpm);
  } else {
    Llift.stop();
    Rlift.stop();
  }
}

void Lcontrol() {
  int pw = 40; // How much power the motors should provide RPM
  // controls the button input for lift
  if (Controller1.ButtonR2.pressing() && BLimitSwitch.pressing() == false) {
    DLcontrol(pw);
  } else if (Controller1.ButtonR1.pressing() && TLimitSwitch.pressing() == false) {
    DLcontrol(-1 * pw);
  } else {
    // Stops the lift if no button is being held
    DLcontrol(0);
  }
}

void Gcode() {
  Brain.Screen.clearScreen();
  Controller1.Screen.clearScreen();
  Brain.Screen.setFont(vex::mono40);
  Brain.Screen.printAt(20, 80, "Gyro");
  Brain.Screen.printAt(20, 120, "Reading: %f", Gyro.value(vex::rotationUnits::deg));
  Controller1.Screen.print("Gyro Reading: %f", Gyro.value(vex::rotationUnits::deg));
}

void MotorStop()
{
  FL.stop();
  BL.stop();
  BR.stop();
  FR.stop();
  Rlift.stop();
  Llift.stop();
  Clawmotor.stop();
}

void ClawControl()
{
  float opened = 30; //Pot values when opened
  float closed = 55; //When Closed
  float spalyed = 0; //When opened all the way, kindof usuless but could be useful later for easier calibration.
  string status = "test"; //Status of claw, c,o,d (close, open, done)
  float degerror = 5.0; //Acceptable degrees of error in the pot
  float Pote = Pot.angle(rotationUnits::deg); // reads pot value. here because of laziness
  int speed = 30; //sets speed of motors.

  //Sets Status
  if(Controller1.ButtonL1.pressing() && status != "opendone")
  {
    status = "opening";
  } else if(Controller1.ButtonL2.pressing() && status != "closeddone")
  {
    status = "closing";
  }
  //Tells motors to spin whens status is appropiate
  if(status == "opening")
  {
    Clawmotor.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
  } else if(status == "closing")
  {
    Clawmotor.spin(vex::directionType::rev, speed, vex::velocityUnits::rpm);
  }
  //Tells motors to stop when they are in the correct position
  degerror /= 2; //divides degerror by 2 so its x degrees total rather than x degrees in either direction.

  if(fabsf(opened - Pote) <= degerror)
  {
    status = "opendone";
    Clawmotor.stop();
  } else if(fabsf(closed - Pote) <= degerror)
  {
    status = "closedone";
    //dont stop the motor so the cubes have constant pressure.
  }

  
  /*When I need to manually control claw
   if(Controller1.ButtonL1.pressing())
  {
    Clawmotor.spin(vex::directionType::fwd, 10, vex::velocityUnits::rpm);
  } else if(Controller1.ButtonL2.pressing())
  {
    Clawmotor.spin(vex::directionType::fwd, -10, vex::velocityUnits::rpm);
  } else
  {
    Clawmotor.stop();
  } 
  */

}
