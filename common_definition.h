#ifndef __COMMON_DEFINITION__
#define __COMMON_DEFINITION__

#include <stdio.h>
#include <stdlib.h>

#define BLACK  2
#define WHITE 47



//戦略を宣言する
//STRATEGY={L_ALL, R_ALL, R_U_TURN, TEST}
#define STRATEGY L_Akino


#define DRIVING_POWER		30

#define STEER	EV3_PORT_C
#define DRIVE_L	EV3_PORT_A
#define DRIVE_R	EV3_PORT_B
#define IR	EV3_PORT_3
#define TOUCH	EV3_PORT_2
#define GYRO    EV3_PORT_4
#define ULTRASONIC EV3_PORT_1
#define DEL 0.004


//for Akino's code
#define MAX_STEERING_ANGLE	360
#define DIAMETER 266.9
#define GYRO_OFFSET 0
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define P_GAIN             2.5F
#define PWM_ABS_MAX          60
//strcture for turn_angle_detection
typedef struct turn_angle_structure {
  float right_motor_current_angle, right_motor_old_angle, right_motor_rotation_angle;
  float left_motor_current_angle, left_motor_old_angle, left_motor_rotation_angle;
  float wheel_across;
  float between_wheels;
  float right_wheel_moving_distance, left_wheel_moving_distance;
  float moving_distance;
  float turning_angle;
  float old_angle;
  float current_angle;
  float original_right_motor_angle;
  float original_left_motor_angle;
} turn_angle_structure;

//constructor. turn_angle_structure_make is used in turn_angle_constructor
turn_angle_structure* turn_angle_constructor(void);
turn_angle_structure* turn_angle_structure_make(void);

//destructor. turn_angle_structure_free is used in self_localization_destructor
void turn_angle_structure_free(turn_angle_structure *tas);

//updating of current coordinates
float turn_angle_detection(turn_angle_structure *tas);




//左エッジ or 右エッジ
enum edge {right,left};
enum yes_or_no {yes, no};


#endif




