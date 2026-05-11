#include "main.h"

// --- Motors ---
// Standard PROS motor groups. (Negative ports reverse the motor)
MotorGroup leftMotors({1, 9});
MotorGroup rightMotors({-2, -10});

MotorGroup intakeMotors({8,-3});
Motor intakeLiftMotor(-5);
Motor trayLiftMotor(7);

// --- Drivetrain Configuration ---
Drivetrain drivetrain(&leftMotors, // left motor group
                      &rightMotors, // right motor group
                      12.0, // track width (12 inches)
                      Omniwheel::NEW_4, // using new 4" omnis
                      200, // drivetrain rpm
                      2); // horizontal drift (2 is standard for normal drive)

// --- Odometry Sensors ---
Imu imu(11);  
OdomSensors sensors(nullptr, // vertical tracking wheel 1
                    nullptr, // vertical tracking wheel 2
                    nullptr, // horizontal tracking wheel 1
                    nullptr, // horizontal tracking wheel 2
                    &imu); // inertial sensor

// --- PID Controllers ---
// Placeholder values - you will need to tune these!
ControllerSettings lateral_controller(10, 0, 3, 3, 1, 100, 3, 500, 20);
ControllerSettings angular_controller(2, 0, 10, 3, 1, 100, 3, 500, 0);

// --- Initialize Chassis ---
Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);


/**
 * A callback function for LLEMU's center button.
 */
void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        lcd::set_text(2, "I was pressed!");
    } else {
        lcd::clear_line(2);
    }
}

/**
 * Runs initialization code.
 */
void initialize() {
    lcd::initialize();
    lcd::set_text(1, "Hello BH Student!");
    lcd::register_btn1_cb(on_center_button);

    //Encoder Limits
    intakeLiftMotor.tare_position(); 
    trayLiftMotor.tare_position();

     // Set brake modes so they hold position
    intakeLiftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    trayLiftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);


    // Calibrate the chassis (calibrates the IMU and starts odometry)
    chassis.calibrate(); 
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

/**
 * Runs the operator control code.
 */
void opcontrol() {
    // Thanks to PROS_USE_SIMPLE_NAMES, we can just say CONTROLLER_MASTER
    Controller master(CONTROLLER_MASTER);



    while (true) {
        
       // --- Manual Drive Logic ---
        int throttle = master.get_analog(ANALOG_LEFT_Y);
        int turn = master.get_analog(ANALOG_RIGHT_X);

        chassis.curvature(leftY, rightX);
        // --- Metric Movement Test (~30cm) ---
        if (master.get_digital_new_press(DIGITAL_X)) {
            chassis.setPose(0, 0, 0); 
            chassis.moveToPoint(0, 11.81, 2000);
            chassis.waitUntilDone();
        }

        


        delay(10); 
    }
}