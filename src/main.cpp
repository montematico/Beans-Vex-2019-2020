#include "main.h"
#include "functions.hpp"
#include "selection.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Utilcode util;
	Visioncode vision;
	util.startup();
	vision.startup();
	//OKAPIinit();
	pros::Task::delay(500);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
}
//Pre auton
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
	/*
    autonSelection == 1 :Blue Front
    autonSelection == 2 : Blue Back
    autonSelection == 3 : Do Nothing
    autonSelection == -1 : Red Front
    autonSelection == -2 : Red Back
    autonSelection == -3 : Do Nothing
    autonSelection == 0 : Skills
*/
	selectorInit();

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	//initializes all the classes and other garbage I use for auton
	Visioncode vision;
	Liftcode lift;
	Clawcode claw;
	Drivecode drive;
	auto chassis = ChassisControllerBuilder() //Init for OKAPI which allows for odometry and other cool functions
	.withMotors( //Creates chassis as x-drive model
		6,  // Top
		-9, // Top right
		-8, // Bottom right
		7   // Bottom left
	)
	// Green gearset, 4 in wheel diam, 11.5 in wheel track
	.withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR}) //Allows for odometry by using onboard encoders
	.withMaxVelocity(100) //tones down speed since it go too fast otherwise
	.withOdometry() //Adds odemetry
	.buildOdometry(); //builds chassis object

	//Begin Auton code
	if (true)
	{
		chassis->moveDistance(5_in); //Deploys claw but leaping forward then stopping

		vision.turncube();
		vision.gocube(); //GOcube calls on turcube if it detects error is too large.
		lift.move(-30);
		claw.open();
		pros::Task::delay(500);
		claw.close();
		lift.move(50);
		pros::Task::delay(1000);
		lift.stop();
	}
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	Clawcode claw;
	Liftcode lift;
	Drivecode drive;
	Utilcode util;
	Visioncode vision;

	controller.rumble("..");
	util.startup();
	vision.startup();

//	vision.turncube();
	vision.gocube();
	while (true)
	{
		//Hands over control of all components to user.
		lift.usrctrl();
		claw.usrctrl();
		drive.usrctrl();
		util.usrctrl();
		pros::Task::delay(10);
	}
}
