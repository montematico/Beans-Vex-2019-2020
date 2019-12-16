#include "vex.h"
extern double dist[2];

void Pturn(int tp)
// Precise Turn for autonomous tp = how many degrees to turn.
{
  while (Gyro.value(vex::rotationUnits::deg) < tp)
  {
    FL.spin(vex::directionType::fwd, 50, vex::velocityUnits::rpm);
    FR.spin(vex::directionType::fwd, 50, vex::velocityUnits::rpm);
    BL.spin(vex::directionType::fwd, 50, vex::velocityUnits::rpm);
    BR.spin(vex::directionType::fwd, 50, vex::velocityUnits::rpm);
  }
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
}

int goCallback(void *pwti)
{
  float *x = (float *)pwti; //Does some magic stuff im far to underqualified to explain/understand
  //For some reason naming *x anything else bricks it, im not sure why but whatever I've had too many breakdowns to care.
  float pw = x[0];
  float dis = x[1];
  float rw = pw * -1;
  double Ydist = 0;

  FL.spin(vex::directionType::fwd, pw, vex::velocityUnits::pct);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::pct);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::pct);
  BR.spin(vex::directionType::fwd, rw, vex::velocityUnits::pct);
  if(dis < 0)
  {   
    pw *= -1;
  }
  while(fabs(Ydist - dis) >= 0.2)
  {
    Ydist = Yencode.position(rotationUnits::deg);
    Ydist = Ydist * (3.1415926535897932/180);
    Ydist *= 1.375;
    std::cout << Ydist << std::endl;
    wait(10,msec);
  }
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
  return 0;
}

void Pgo(float pw, float dis)
{
  //Im not sure how to pass multiple arguments to tasks
  //So I put it into an array which I send over
  float pwti [2] = {pw,dis};
  task robogo( goCallback, (void *)&pwti);
}

int Pstrafecallback(void *pwti) 
{
  float *x = (float *)pwti; //Does some magic stuff im far to underqualified to explain/understand
  //For some reason naming *x anything else bricks it, im not sure why but whatever I've had too many breakdowns to care.
  float pw = x[0];
  float dis = x[1];
  double Xdist = 0;
  if(dis < 0)
  {
    pw *= -1;
  }
  float rw = pw * -1;
  FL.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  BR.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  while(fabs(Xdist-dis) >= 0.2)
  {
    Xdist = Xencode.position(rotationUnits::deg);
    Xdist = Xdist * (3.1415926535897932/180); //Converts to radiians
    Xdist *= 1.375; //Radius of wheel
    std::cout << Xdist << std::endl;
    wait(10,msec);
  }
  std::cout << "oop" << std::endl;
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
  return 0;
}
void Pstrafe(float pw, float dis)
{
  //Im not sure how to pass multiple arguments to tasks
  //So I put it into an array which I send over
  float pwti [2] = {pw,dis};
  task robostrafe( Pstrafecallback, (void *)&pwti);
}

void Startup() {

  // Calibrates Gyro
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Calibrating...");
  Controller1.Screen.newLine();
  Controller1.rumble(".");

  Brain.Screen.setFont(vex::mono40);
  Brain.Screen.printAt(1, 40, "Calibrating...");
  Gyro.startCalibration();
  wait(0, sec);
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(1, 40, "Done");
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Done");
  Controller1.rumble("..");
}
void motorset()
{
  // Sets motor settings
  Clawmotor.setMaxTorque(100, percentUnits::pct);
  Xencode.resetRotation();
  Yencode.resetRotation();
  Llift.setStopping(hold);
  Rlift.setStopping(hold);
  BL.setStopping(coast);
  BR.setStopping(coast);
  FL.setStopping(coast);
  FR.setStopping(coast);
  Clawmotor.setStopping(hold);
}

void go(int pw) {
  int rw = pw * -1;
  // Precice go for autonomous pw = power (rpm) ti = time (seconds)
  FL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  BR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
}
void halt()
{
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
}
void Autoclaw(char x)
{
  float speed = 30.0;
  if(x == 'c') //Check if you're doing the wirhgt math here.
  {
    Clawmotor.spin(vex::directionType::fwd, 60, vex::velocityUnits::rpm);
  } else if(x == 'o')
  {
    Clawmotor.spin(vex::directionType::rev, speed, vex::velocityUnits::rpm);
  } else if(x == 's')
  {
    Clawmotor.stop();
  }
}
