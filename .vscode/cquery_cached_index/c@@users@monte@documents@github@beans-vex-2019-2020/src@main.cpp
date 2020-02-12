#include "main.h"
#include "functions.hpp"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	Utilcode util;
	VisionCode vision;
	util.startup();
	vision.startup();
	//OKAPIinit();
	pros::lcd::initialize();
	pros::Task::delay(500);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	pros::lcd::initialize();
	pros::lcd::set_text(2, "Get B E A N E D!");
	pros::Task::delay(100);
	pros::lcd::set_text(4, "Get B E A N E D!");
	pros::Task::delay(100);
	pros::lcd::set_text(6, "Get B E A N E D!");
	pros::Task::delay(100);
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
	pros::lcd::clear();
	pros::lcd::set_text(5,"Ready!");
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
	Liftcode lift;
	Clawcode claw;
	Drivecode drive;
	auto chassis = ChassisControllerBuilder()
	.withMotors(
	6,  // Top left
	-9, // Top right
	-8, // Bottom right
	7   // Bottom left
	)
	// Green gearset, 4 in wheel diam, 11.5 in wheel track
	.withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR})
	.withMaxVelocity(100)
	.withOdometry()
	.buildOdometry();
claw.close();
chassis->moveDistance(8_in);
lift.move(-80);
chassis->turnAngle(270_deg);
lift.move(0);
chassis->moveDistance(1_ft);
claw.open();
pros::Task::delay(500);
claw.stop();
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
void opcontrol() {
	Clawcode claw;
	Liftcode lift;
	Drivecode drive;
	Utilcode util;
	VisionCode vision;

	controller.rumble("..");
	util.startup();
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
