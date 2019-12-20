#include "main.h"
#include "motorconfig.hpp"

double dist[2]; //Making distane variable global so all code can pull from it.

// Functions makes the code readable.
// Making Drive Train Variables Global

// More Variables for gnocchi code
int Goff = 0; // Gyroscope offset, set to zero because its non functional
void DriveTrainCallback(void* param)
{
  //void* param
  while(true)
  {
    double FLI = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    double BLI = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    double FRI = (-1 * controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    double BRI = (-1 * controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);

    FL.move(FLI);
    BL.move(BLI);
    FR.move(FRI);
    BR.move(BRI);
    delay(2);
  }
}
void DriveTrain() // Contains code for driving motors
{
   double FLI = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
   double BLI = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
   double FRI = (-1 * controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
   double BRI = (-1 * controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X) + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);

   FL.move(FLI);
   BL.move(BLI);
   FR.move(FRI);
   BR.move(BRI);
   pros::delay(2);
}

void NorthTurn(int dir)
{
  int pwr = 50; //turn rate 0-100 %

  while ( fabs(dir - Gyro.value(vex::rotationUnits::deg)) >= 3)
  {

    FL.move(pwr);
    FR.move(pwr);
    BL.move(pwr);
    BR.move(pwr);
  }
  FL.move(0);
  FR.move(0);
  BL.move(0);
  BR.move(0);
}

void Ncheck()
{
  if(controller.get_digital(E_CONTROLLER_DIGITAL_UP)) {
    NorthTurn(0);}
  else if(Controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT)){
    NorthTurn(270);}
  else if(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT)){
    NorthTurn(90);}
  else if(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN)){
    NorthTurn(180);}
  else if(controller.get_digital(E_CONTROLLER_DIGITAL_Y))
    {
      Gyro.setHeading(0,degrees); //Sets new "north" if Y button is pressed
    }
}

void DLcontrol(double pwr) {
  // Synchronizes motors for lift
  // uses pwr as a -100-100 integer for controlling both lift motors
  //Also used in autonamous code to raise and lower lift.
  if (pwr != 0) {
    Llift.move(pwr);
    Rlift.move(pwr);
  } else {
    Llift.move(0);
    Rlift.move(0);
  }
}

void Lcontrol() {
  double pw = 60.00; // How much power the motors should provide RPM
  // controls the button input for lift
  if (controller.get_digital(E_CONTROLLER_DIGITAL_R2))
   {
    DLcontrol(pw);
  } else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)) {
    DLcontrol(-1 * pw);
  } else
  {
    // Stops the lift if no button is being held
    DLcontrol(0);
  }
}

void MotorStop()
{
  FL.move(0);
  BL.move(0);
  BR.move(0);
  FR.move(0);
  Rlift.move(0);
  Llift.move(0);
  Clawmotor.move(0);
}
/*
void ClawControl()
{
  float opened = 30; //Pot values when opened
  float closed = 55; //When Closed
  float spalyed = 0; //When opened all the way, kindof usuless but could be useful later for easier calibration.
  string status = "test"; //Status of claw, c,o,d (close, open, done)
  float degerror = 5.0; //Acceptable degrees of error in the pot
  float Pote = Pot.angle(rotationUnits::deg); // reads pot value. here because of laziness
  int speed = 30; //sets speed of motors.


  if(Controller1.ButtonL1.pressing() && Pote <= closed) //Check if you're doing the wirhgt math here.
  {
    Clawmotor.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
  } else if(Controller1.ButtonL2.pressing() && Pote <= opened)
  {
    Clawmotor.spin(vex::directionType::rev, speed, vex::velocityUnits::rpm);
  } else
  {
    Clawmotor.stop();
  }

*/ //DELETE THIS IDIOT LOOK HERE LOOK HERE LOOK HERE LOOK HERE LOOK HERE

  //When I need to manually control claw
  /*
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
  /* MORE CODE GONE
}
void encoderreturn()
{
  //Returns an array with x,y distance travelled in inches.
  double Xdist = Xencode.position(rotationUnits::deg);
  double Ydist = Yencode.position(rotationUnits::deg);
  //Converts degrees to radiians.
  Xdist = Xdist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  Ydist = Ydist * (3.1415926535897932/180); //Nasa only used 15, our Superior code uses 16. :)
  Xdist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  Ydist *= 1.375; // ArcLength = θ * r.  θ in radii.
  double dist[2] = {Xdist, Ydist};
  std::cout << "X:" << std::endl;
  std::cout << dist[0] << std::endl;
  std::cout << "Y:" << std::endl;
  std::cout << dist[1] << std::endl;
}
double Xreturn()
{
  //Returns an array with x,y distance travelled in inches.
  double Xdist = Xencode.position(rotationUnits::deg);
  //Converts degrees to radiians.
  Xdist = Xdist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  //Nasa only used 15, our Superior code uses 16. :)
  Xdist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  // ArcLength = θ * r.  θ in radii.
  return Xdist;
}

double Yreturn()
{
{
  //Returns an array with x,y distance travelled in inches.
  double Ydist = Yencode.position(rotationUnits::deg);
  //Converts degrees to radiians.
  Ydist = Ydist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  //Nasa only used 15, our Superior code uses 16. :)
  Ydist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  // ArcLength = θ * r.  θ in radii.
  return Ydist;
}
}

void Gcode() {

    //Returns an array with x,y distance travelled in inches.
  double Xdist = Xencode.position(rotationUnits::deg);
  double Ydist = Yencode.position(rotationUnits::deg);
  //Converts degrees to radiians.
  Xdist = Xdist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  Ydist = Ydist * (3.1415926535897932/180); //Nasa only used 15, our Superior code uses 16. :)
  Xdist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  Ydist *= 1.375; // ArcLength = θ * r.  θ in radii.
  double dist[2] = {Xdist, Ydist};

  Brain.Screen.clearScreen();
  Controller1.Screen.clearScreen();
  Brain.Screen.setFont(vex::mono20);
  Brain.Screen.printAt(20, 40, "Pot Reading %f",Pot.value(vex::rotationUnits::deg));
  Brain.Screen.printAt(20, 80, "X: %f Y: %f",dist[0], dist[1] );
}
*/
