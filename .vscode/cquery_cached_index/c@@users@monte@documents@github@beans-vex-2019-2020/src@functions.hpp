#include "main.h"
float prate = 1.00;
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
         Clawmotor.move(speed * prate);
      } else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2))
      {
        Clawmotor.move(-speed * prate);
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
    FL.move(FLI * prate);
    BL.move(BLI * prate);
    FR.move(FRI * prate);
    BR.move(BRI * prate);
    pros::delay(2);
  }
};

class Utilcode
{
public:
  bool precise = false;
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
  void usrctrl() //Sets the robot into precise mode where everying runs at half speed
  {
    if(controller.get_digital(E_CONTROLLER_DIGITAL_DOWN))
    {
      prate = 0.50;
    } else if (controller.get_digital(E_CONTROLLER_DIGITAL_UP))
    {
      prate = 1.00;
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

class Liftcode //Code concerning the lift
{
private:
  float speed = 127; //Speed that the lift should run at when controlled (-127-127)
public:
  void move(double pwr)
  {
    //The manual lift control code. for auton
    Llift.move(pwr);
    Rlift.move(pwr);
  }
  void usrctrl() //controls lift with controller
  {
    if(controller.get_digital(E_CONTROLLER_DIGITAL_R2))
    {
      Llift.move(speed * prate);
      Rlift.move(speed * prate);
    } else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1))
    {
      Llift.move(-speed * prate);
      Rlift.move(-speed * prate);
    } else
    {
      //stops the motors if no button is being pressed
      Llift.move(0);
      Rlift.move(0);
    }
  }
};
//Wow, you read (or scrolled) through all the code, nice!
//I self taught myself c++ and this code has iterated through many versions.
//The first version was in VCS and had no functions.
//The second version was all functions with 3 different header files and many redudant functions
//The third and (hopefully) final version is this. everything is in classes and there is nearly no redundancy.
//Although I kinda miss thie overly compelex auton functions I wrote for V.2;
//Oh well, I know its not perfect but its mine ☺.
