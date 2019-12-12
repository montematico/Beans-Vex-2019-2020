/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Sun Oct 06 2019                                           */
/*    Description:  This program will run a tasks parallel (at the same time  */
/*                  to main.                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// myTaskCallback is a callback function that can be registered to a task. In
// this program, it is registered to 'myTask'.
int myTaskCallback() {
  int count = 0;
  while (true) {
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("myTaskCallback has iterated %d times", count);
    count++;
    wait(25, msec);
  }
  // A task's callback must return an int, even though the code will never get
  // here. You must return an int here. Tasks can exit, but this one does not.
  return 0;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Declare and assign myTask's callback to 'myTaskCallback'.
  // The task will start as soon as this command is called.
  task myTask = task(myTaskCallback);

  // Print from the main task to show that it is running at the same time as
  // 'myTaskCallback'.
  int count = 0;
  while (true) {
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("main has iterated %d times", count);
    count++;
    wait(25, msec);
  }
}
