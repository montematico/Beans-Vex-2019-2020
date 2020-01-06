#include "main.h"


extern double dist[2];
void OKAPIinit()
{
  //Creats drivetrain for OKAPI for auton
  //Defining what port motors are in
  const int FLM = 6;
  const int BLM = 7;
  const int FRM = 8;
  const int BRM = 9;
  //Gives wheel diameter and chassis width for auto
  const auto Wheel_Diameter = 3.25_in;
  const auto Chassis_width = 17_in;

  auto chassis = okapi::ChassisControllerFactory::create(FLM,FRM,BLM,BRM,
    okapi::AbstractMotor::gearset::green,{Wheel_Diameter,Chassis_width});
}

void motorset()
{
  //Motors
  Clawmotor.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  Llift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  Rlift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
  BL.set_brake_mode(E_MOTOR_BRAKE_COAST);
  BR.set_brake_mode(E_MOTOR_BRAKE_COAST);
  FL.set_brake_mode(E_MOTOR_BRAKE_COAST);
  FR.set_brake_mode(E_MOTOR_BRAKE_COAST);
  //Encoders
  Xencode.reset();
  Yencode.reset();

}


void Autoclaw(char x)
{
  float speed = 30.0;
  if(x == 'c')
  {
    Clawmotor.move(speed);
  } else if (x == 'o')
  {
    Clawmotor.move(-speed);
  }
   else if(x == 's')
  {
    Clawmotor.move(0);
  }
}
