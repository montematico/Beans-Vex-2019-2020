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

void Pgo(int pw, int ti) {
  ti = 1000 * ti;
  int rw = pw * -1;
  // Precice go for autonomous pw = power (rpm) ti = time (seconds)
  FL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  FR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  BL.spin(vex::directionType::fwd, pw, vex::velocityUnits::rpm);
  BR.spin(vex::directionType::fwd, rw, vex::velocityUnits::rpm);
  vex::task::sleep(ti);
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();
}

void Startup() {
  // Sets motor settings
  Llift.setStopping(brake);
  Rlift.setStopping(brake);
  BL.setStopping(coast);
  BR.setStopping(coast);
  FL.setStopping(coast);
  FR.setStopping(coast);

  // Calibrates Gyro
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Calibrating...");
  Controller1.Screen.newLine();
  Controller1.rumble(".");

  Brain.Screen.setFont(vex::mono40);
  Brain.Screen.printAt(1, 40, "Calibrating...");
  Gyro.startCalibration();
  vex::task::sleep(1500);
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(1, 40, "Done");
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Done");
  Controller1.rumble("..");
  vex::task::sleep(300);
}

void go(int dir, int pwr, int) {
  int Goff = 0; // nonfunctional maybe later
  int AFL;
  int ABL;
  int AFR;
  int ABR;
  int x = 10 * cos(dir);
  int y = 10 * sin(dir);

  AFL = y + (1 * y) - Goff;
  ABL = y - (1 * y) - Goff;
  AFR = (-1 * y) + (1 * y) - Goff;
  ABR = (-1 * y) - (1 * y) - Goff;

  FL.spin(vex::directionType::fwd, AFL, vex::velocityUnits::pct);
  BL.spin(vex::directionType::fwd, ABL, vex::velocityUnits::pct);
  FR.spin(vex::directionType::fwd, AFR, vex::velocityUnits::pct);
  BR.spin(vex::directionType::fwd, ABR, vex::velocityUnits::pct);
}