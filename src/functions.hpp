#include "main.h"
float prate = 1.00;
// Functions makes the code readable.
//CLASSES MAKE IT COOLER
// Making Drive Train Variables Global

class Utilcode
{
public:
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
  }
  float get_value() //Fixes ultrasonic value to cm and substracts 10 mm since it has an error.
  {
     return (((Cubesense.get_value()-10)/10)/2.54);
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
  void autonrecord()
  {
    clock_t  t;
    t = clock();
    printf("%lu\n", t);
    printf("FR: %d\n", FR.get_voltage());
    printf("FL: %d\n", FL.get_voltage());
    printf("BR: %d\n", BR.get_voltage());
    printf("BL: %d\n", BL.get_voltage());

  }
};

class Drivecode
{
public:
  void againstwall()
  {
    while(fabs(Lsense.get_value() - Rsense.get_value()) >= 1 )
    {
      //Calculates all the values for the PID loop
      std::map<std::string,float> tuner = {{"KP", 0.3}, {"KI", 0.1}, {"KD", 5}};
      float error = fabs(Lsense.get_value() - Rsense.get_value());
      float integral = integral + error;
      if (error <= 10 && fabs(error) >= 300) integral = 0;
      float prevError = error;
      float derivative = error - prevError;
      float power = error*tuner.at("KP") + integral*tuner.at("KI") + derivative*tuner.at("KD");
      pros::Task::delay(15);
    }
    this->stop();
  }
  void stop()
  {
    FL.move(0);
    BL.move(0);
    BR.move(0);
    FR.move(0);
  }
  void gofw(float pw, float ti = -1)
  {
    pw *= 1.27;
    FL.move(pw);
    BL.move(pw);
    BR.move(-pw);
    FR.move(-pw);
    if(ti != -1)
    {
      pros::Task::delay(ti*1000);
      this->stop();
    }
  }
  void gosw(float pw, float ti = -1)
  {
    pw *= 1.27;
    FL.move(pw);
    BL.move(pw);
    FR.move(pw);
    BR.move(pw);
    if(ti != -1)
    {
    pros::Task::delay(ti*1000);
    this->stop();
    }
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
  void turnleft(float pw,float ti=-1)
  {
    //Simple functions allow to turn, optional time param for auton
    FL.move(pw);
    FR.move(pw);
    BL.move(pw);
    BR.move(pw);
    if(ti != -1)
    {
      pros::Task::delay(ti*1000);
      this->stop();
    }
  }
  void turnright(float pw,float ti=-1)
  {
    FL.move(-pw);
    FR.move(-pw);
    BL.move(-pw);
    BR.move(-pw);
    if(ti != -1)
    {
      pros::Task::delay(ti*1000);
      this->stop();
    }
  }
};

class Clawcode  : public Utilcode , public Drivecode
{
private:
  const float speed = 60; //sets speed of claw
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
    bool Csense(bool debug = false) //Sh**ty little function that return true if it senses a cube in the claw & false if it doesnt
    { //debug flag outs puts it in a loop that continually returns the value and if cube is seen
      float clawSpan = 14; //How wide the claw is when fully extended in _in
      Cdebug:
      if(this->get_value() > 14)
      {
        return false;
        printf("No Cube Sensed :(\n");
      } else
      {
        return true;
        printf("Cube sensed!\n");
      }
      printf("Distance:%f\n",this->get_value());
      if(debug) goto Cdebug;
    }
  };



class Liftcode //Code concerning the lift
{
private:
  const float speed = 127; //Speed that the lift should run at when controlled (-127-127)
public:
  void move(double pwr)
  {
    //The manual lift control code. for auton
    Llift.move(pwr);
    Rlift.move(pwr);
  }
  void stop()
  {
    Llift.move(0);
    Rlift.move(0);
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



class Visioncode : public Clawcode
{
private:
  float error = 20;
protected:
  vision_object_s_t cube = vision_sensor.get_by_size(0);
  int coord[2] ={cube.x_middle_coord, cube.y_middle_coord};
public:
  void startup()
  {

    //Sets preset signatures
     vision_sensor.clear_led();
     vision_sensor.set_led(COLOR_YELLOW);
     vision_sensor.signature_from_utility(1, -5761, -4079, -4920, -2497, -449, -1474, 3.000, 0);; //Green Cube
     vision_sensor.signature_from_utility(2, 6943, 8831, 7886, -2641, -1471, -2056, 1.400, 0); //Orange Cube
     vision_sensor.clear_led();
     printf("Color Signatures set\n");
     vision_sensor.set_led(COLOR_GREEN);
  }

  //Turns to find a cube until error is below and allowed limit
  bool turncube(bool debug = false)
  {
    //setting a bunch of variables to their appropiate defaults and initializing OKAPI
  //auto chassis = ChassisControllerBuilder().withMotors(6,-9,-8,7).withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR}).withMaxVelocity(100).withOdometry().buildOdometry();
    cube = vision_sensor.get_by_size(0);
    coord [0] = cube.x_middle_coord;
    coord [1] = cube.y_middle_coord;
    std::map<std::string,float> tuner = {{"KP",0.7}, {"KI", 0}, {"KD", 0.5}}; //A bunch of PID tuning varaibles. its a MAP because im stupid
    printf("x:%d y:%d \n",cube.x_middle_coord, cube.y_middle_coord);

    while(true) //Change second value to make more or less precice before exiting loop
    {
      //Gets the nth largest object the camera sees and saves it to the coord array
      cube = vision_sensor.get_by_size(0);
      coord [0] = cube.x_middle_coord;
      coord [1] = cube.y_middle_coord;
      printf("x:%d y:%d \n",coord[0], coord[1]);

      //Calculates all the values for the PID loop
      error = 158 - coord[0];
      float integral = integral + error;
      if (error <= 10 || fabs(error) >= 300) integral = 0;
      float prevError = error;
      float derivative = error - prevError;
      float power = error*tuner.at("KP") + integral*tuner.at("KI") + derivative*tuner.at("KD");
      printf("turning");
      if(!debug)
      {
        if (error <= 5)
        {
          this->stop();
          for (int i = 0; i > 10; i++)
          {
            //This function tests that readings from camera arent a fluke, so when a small value is detected it waits for 100ms of continues inbound readings
            //Before proceding, otherwise it breaks from the loop and continues looking for the cube.
            if(this->get_value() > 5)
            {
              goto turnFluke; //I could use a bool and test to see how many loops to break from but its easier with a goto
            }
            pros::Task::delay(10);
          }
          return true;
        }
      }
    turnFluke:
    printf("Error:%f, Power: %f\n",error,power);
    pros::Task::delay(15);
    }
  }
  bool turngo(bool debug = false)
  {
    //setting a bunch of variables to their appropiate defaults and initializing OKAPI
  //auto chassis = ChassisControllerBuilder().withMotors(6,-9,-8,7).withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR}).withMaxVelocity(100).withOdometry().buildOdometry();
    cube = vision_sensor.get_by_size(0);
    coord [0] = cube.x_middle_coord;
    coord [1] = cube.y_middle_coord;
    std::map<std::string,float> tuner = {{"KP",0.7}, {"KI", 0}, {"KD", 0.5}}; //A bunch of PID tuning varaibles. its a MAP because im stupid
    printf("x:%d y:%d \n",cube.x_middle_coord, cube.y_middle_coord);

    while(true) //Change second value to make more or less precice before exiting loop
    {
      //Gets the nth largest object the camera sees and saves it to the coord array
      cube = vision_sensor.get_by_size(0);
      coord [0] = cube.x_middle_coord;
      coord [1] = cube.y_middle_coord;
      printf("x:%d y:%d \n",coord[0], coord[1]);

      //Calculates all the values for the PID loop
      error = 158 - coord[0];
      float integral = integral + error;
      if (error <= 10 || fabs(error) >= 300) integral = 0;
      float prevError = error;
      float derivative = error - prevError;
      float power = error*tuner.at("KP") + integral*tuner.at("KI") + derivative*tuner.at("KD");
      printf("turning");

      if(error < 5)
      {
        if(!this->Csense())
        {
          return true;
        } else
        {
          this->gofw(50);
        }
      }
      printf("Error:%f, Power: %f\n",error,power);
      pros::Task::delay(15);
    }
  }
  bool gocube(bool debug = false) //Should measure distance to cube and go pick it up.
  {
    float tuner[3] = {2.5,0,0};
    float toCube = 2;

    //If error gets too big code will exit
    //MAKE THIS A PID LOOP IF YOU WANT or dont, I dont care as long as it works.
    while(true) //This is a little wack so the turbcube doesnt exit back into the goloop creating a *possible* recursion
    {
      error = fabs(toCube - this->get_value());
      float integral = integral + error;
      if (error == 0 || fabs(error) <= toCube) integral = 0;
      if (error >= 90) integral = 0;
      float prevError = error;
      float derivative = error - prevError;
      float power = error *tuner[0] + integral*tuner[1] + derivative*tuner[2];

      if(this->get_value() <= toCube) this->gofw(power);
      else if(!debug)
      {
        this->stop();
        for (int i = 0; i > 10; i++)
        {
          //This function tests that readings from ultrasonic arent a fluke, so when a small value is detected it waits for 100ms of inbound readings
          //Before proceding, otherwise it breaks from the loop and continues travelling towards the cube.
          if(this->get_value() > toCube) goto gofluke; //I could use a bool and test to see how many loops to break from but its easier with a goto
          pros::Task::delay(10);
        }
        //Only executes if the value wasnt a fluke
        printf("%fin away, exiting",this->get_value());
        return true; //true means that it is placed infront of a cube.
      }
      coord[0] = cube.x_middle_coord;
      if(fabs(158 - coord[0]) >= 7 && !debug)
      {
      printf("Cube no longer ahead, exiting");
      return false; //tests if the cube is still within the accatable range for grabbing, if not it returns false and exits
      }
      gofluke:
      printf("Distance:%f Error:%f, Power: %f\n",this->get_value(),error,power);
      pros::Task::delay(15);
    }
  }
  void test()
  {
    //DO NOT USE IN PRODUCTION CODE
    //prints the distance in inches and the raw value from the sonar sensor. Used for debugging.
      while(true)
      {
        printf("Mod Dist: %f, RawDist: %d\n",this->get_value(), Cubesense.get_value());
        pros::Task::delay(5000);
      }
  }
};



//Wow, you read (or skipped) through all the code, nice!
//I self taught myself c++ and this code has iterated through many versions.
//The first version was in VCS and had no functions.
//The second version was all functions with 3 different header files and many redudant functions
//The third and (hopefully) final version is this. everything is in classes and there is nearly no redundancy.
//Although I kinda miss thie overly compelex auton functions I wrote for V.2.
//Oh well, I know its not perfect but its mine ☺.
