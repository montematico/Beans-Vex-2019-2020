#include "main.h"
#include "functions.hpp"
#include "autofunct.hpp"
/*
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	motorset();
	//OKAPIinit();
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::Task::delay(500);

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	motorset();
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
motorset();
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

	// Move 1 meter to the first goal
chassis->moveDistance(1_m);
// Turn 90 degrees to face second goal
chassis->turnAngle(90_deg);
// Drive 1 and a half feet toward second goal
chassis->moveDistance(1.5_ft);
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
	controller.rumble("..");
	motorset();
	int param = 5;
  Task DriveTrain(DriveTrainCallback, &param, "");

	while (true)
	{
		Ncheck();
		Lcontrol();
		ClawControl();
		pros::delay(20);
	}
}
