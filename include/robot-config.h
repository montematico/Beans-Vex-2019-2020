using namespace vex;

extern brain Brain;

// VEXcode devices
extern limit BLimitSwitch;
extern limit TLimitSwitch;
extern gyro Gyro;
extern motor FL;
extern motor BL;
extern motor BR;
extern motor FR;
extern motor Rlift;
extern motor Llift;
extern controller Controller1;
extern motor Clawmotor;
extern pot Pot;
extern encoder Xencode;
extern encoder Yencode;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );