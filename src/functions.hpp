#include "main.h"

// Functions makes the code readable.
//CLASSES MAKE IT COOLER
// Making Drive Train Variables Global

class Clawcode
{
private:
  float speed = 60; //sets speed of claw
public:
    char status; //can be read by other functions to see if motor is opening stopped or closed.
    void open()
    {
      status = 'o';
      Clawmotor.move(-speed);
    }
    void close()
    {
      status = 'c';
      Clawmotor.move(speed);
    }
    void stop()
    {
      status = 's';
      Clawmotor.move(0);
    }
    void usrctrl()
    {
      if(controller.get_digital(E_CONTROLLER_DIGITAL_L1))
      {
         Clawmotor.move(speed);
      } else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2))
      {
        Clawmotor.move(-speed);
      } else
      {
        Clawmotor.move(0);
      }
    }
  };

class Drivecode
{
public:
  void stop()
  {
    FL.move(0);
    BL.move(0);
    BR.move(0);
    FR.move(0);
  }
  void gofw(float pw, float ti)
  {
    pw *= 1.27;
    FL.move(-pw);
    BL.move(pw);
    BR.move(-pw);
    FR.move(pw);
    pros::Task::delay(ti*1000);
    this->stop();
  }
  void gosw(float pw, float ti)
  {
    pw *= 1.27;
    FL.move(pw);
    BL.move(pw);
    FR.move(pw);
    BR.move(pw);
    pros::Task::delay(ti*1000);
    this->stop();
  }
  void usrctrl()
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
};

class Utilcode
{
public:
  bool precise = false;
  float prate;
    void startup() //Performs all neccecary startup procedures like setting brake modes and resetting encoders
  {
    //Sets motor brake mode
    Clawmotor.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    Llift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    Rlift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
    BL.set_brake_mode(E_MOTOR_BRAKE_COAST);
    BR.set_brake_mode(E_MOTOR_BRAKE_COAST);
    FL.set_brake_mode(E_MOTOR_BRAKE_COAST);
    FR.set_brake_mode(E_MOTOR_BRAKE_COAST);
    //Resets all sensors to 0
    Xencode.reset();
    Yencode.reset();
  }
  void pchecker() //Sets the robot into precise mode where everying runs at half speed
  {
    if(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN))
    {
      prate = 0.5;
    } else if (controller.get_digital(E_CONTROLLER_DIGITAL_UP))
    {
      prate = 1;
    }
  }
  void abort() //Function that kills all motors if shit really hits the fan
  {
    BL.move(0);
    BR.move(0);
    FL.move(0);
    FR.move(0);
    Llift.move(0);
    Rlift.move(0);
    Clawmotor.move(0);
  }
  void OKAPIinit()
  {
    //Initiales the okapi library to allow for better auton control.
    auto chassis = ChassisControllerBuilder()
    .withMotors(
    6,  // Top left
    8, // Top right (reversed)
    9, // Bottom right (reversed)
    7   // Bottom left
    )
    // Green gearset, 4 in wheel diam, 11.5 in wheel track
    .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
    .build();
  }
};

class Liftcode
{
private:
  float speed = 127; //Speed that the lift should run at when controlled (-127-127)
public:
  void move(double pwr)
  {
    //The manual lift control code.
    Llift.move(pwr);
    Rlift.move(pwr);
  }
  void usrctrl()
  {
    if(controller.get_digital(E_CONTROLLER_DIGITAL_R2))
    {
      Llift.move(speed);
      Rlift.move(speed);
    } else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1))
    {
      Llift.move(-speed);
      Rlift.move(-speed);
    } else
    {
      //stops the motors if no button is being pressed
      Llift.move(0);
      Rlift.move(0);
    }
  }
};
/*
//TODO either add the encoders back or nuke this function.
void encoderreturn()
{
  //Returns an array with x,y distance travelled in inches.
  double Xdist = Xencode.get_value();
  double Ydist = Yencode.get_value();
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
  double Xdist = Xencode.get_value();
  //Converts degrees to radiians.
  Xdist = Xdist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  //Nasa only used 15, our Superior code uses 16. :)
  Xdist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  // ArcLength = θ * r.  θ in radii.
  return Xdist;
}

double Yreturn()
{
  //Returns an array with x,y distance travelled in inches.
  double Ydist = Yencode.get_value();
  //Converts degrees to radiians.
  Ydist = Ydist * (3.1415926535897932/180); //This uses more digits of pi than NASA lmao.
  //Nasa only used 15, our Superior code uses 16. :)
  Ydist *= 1.375; //Uses ark length formula to calculate distance travelled in inches
  // ArcLength = θ * r.  θ in radii.
  return Ydist;
}
*/
