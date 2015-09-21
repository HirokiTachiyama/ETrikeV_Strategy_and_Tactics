#ifndef TURN_ANGLE_DETECTION
#define TURN_ANGLE_DETECTION


#include "common_definition.h"

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

#endif //TURN_ANGLE_DETECTION
