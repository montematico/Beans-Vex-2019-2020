#include "vex.h"

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
  float ti = x[1];
  float rw = pw * -1;
  std::cout << pw << std::endl;
  std::cout << "PW above" << std::endl;
  std::cout << ti << std::endl;


  FL.spin(vex::directionType::fwd, pw, vex::velocityUnits::pct);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::pct);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::pct);
  BR.spin(vex::directionType::fwd, rw, vex::velocityUnits::pct); 
  wait(ti,sec);
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
  return 0;
}

void Pgo(float pw, float ti) 
{
  //Im not sure how to pass multiple arguments to tasks
  //So I put it into an array which I send over
  float pwti [2] = {pw,ti};
  task robo( goCallback, (void *)&pwti);
}


void Pstrafe(int pw, double ti) {
  //ti = 1000 * ti;
  int rw = pw * -1;
  // Precice go for autonomous pw = power (rpm) ti = time (seconds)
  FL.spin(vex::directionType::rev, pw, vex::velocityUnits::rpm);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  BR.spin(vex::directionType::rev, rw, vex::velocityUnits::rpm);
  wait(ti, sec);
 // vex::task::sleep(ti);
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
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
  Clawmotor.setMaxTorque(80.08, percentUnits::pct);
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