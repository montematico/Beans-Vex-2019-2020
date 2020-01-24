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
void halt()
{
  FL.move(0);
  BL.move(0);
  BR.move(0);
  FR.move(0);
}
void go(double pw,double ti)
{
  pw *= 1.27;
  FL.move(-pw);
  BL.move(pw);
  FR.move(pw);
  BR.move(-pw);
  pros::lcd::set_text(1, "Moving forward");
  pros::Task::delay(ti*1000);
  halt();
}
void strafe(double pw,double ti)
{
  pw *= 1.27;
  FL.move(pw);
  BL.move(pw);
  FR.move(pw);
  BR.move(pw);
  pros::lcd::set_text(1, "strafing");
  pros::Task::delay(ti*1000);
  halt();
}
void go_caller(void* param)
 {
  double *pwti = (double *)pwti;
  pwti[0] *= 1.27;
  FL.move(-pwti[0]);
  BL.move(pwti[0]);
  FR.move(pwti[0]);
  BR.move(-pwti[0]);
  pros::lcd::set_text(1, "Moving forward (m)");
  pros::Task::delay(pwti[1]*1000);
  halt();
}
void Mgo(double pw,double ti) {
  std::printf("Mgo is running");
  double pwti[2] = {pw,ti};
  Task gocaller(go_caller, &pwti, "Gocaller");
}

void str_caller(void* param)
 {
  double *pwti = (double *)pwti;
  FL.move(-pwti[0]);
  BL.move(pwti[0]);
  FR.move(-pwti[0]);
  BR.move(pwti[0]);
  pros::Task::delay(pwti[1]);
  halt();
}
void Mstr(double pw,double ti) {
  double pwti[2] = {pw,ti};
  Task strcaller(str_caller, &pwti, "STRcaller");
}
