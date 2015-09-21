#include "turn_angle_detection.h"
#include "ev3api.h"
#include "common_definition.h"

#define STEER	EV3_PORT_C
#define DRIVE_L	EV3_PORT_A
#define DRIVE_R	EV3_PORT_B
#define IR		EV3_PORT_3
#define TOUCH	EV3_PORT_2

/***********************************************
*	走行体の回転角度の検知
***********************************************/

//コンストラクタ
turn_angle_structure* turn_angle_constructor(){
  //setting sensor value of radius to 0
  ev3_motor_reset_counts(DRIVE_R); //right motor
  ev3_motor_reset_counts(DRIVE_L); //left motor
  return turn_angle_structure_make();
}

//回転角度検知用の構造体作成
turn_angle_structure *turn_angle_structure_make(void) {
  turn_angle_structure *new_turn_angle_structure =
    (turn_angle_structure*)malloc(sizeof(turn_angle_structure));
  new_turn_angle_structure->right_motor_current_angle = ev3_motor_get_counts(DRIVE_R);
  new_turn_angle_structure->left_motor_current_angle = ev3_motor_get_counts(DRIVE_L);
  new_turn_angle_structure->right_motor_old_angle = 0;
  new_turn_angle_structure->left_motor_old_angle  = 0;
  new_turn_angle_structure->right_motor_rotation_angle = 0;
  new_turn_angle_structure->left_motor_rotation_angle = 0;
  new_turn_angle_structure->wheel_across = 8.0;
  new_turn_angle_structure->between_wheels = 12.0;
  new_turn_angle_structure->moving_distance = 0;
  new_turn_angle_structure->turning_angle = 0;
  new_turn_angle_structure->old_angle = 0;
  new_turn_angle_structure->current_angle = 0;
  new_turn_angle_structure->original_right_motor_angle = 0;
  new_turn_angle_structure->original_left_motor_angle = 0;
  return new_turn_angle_structure;
}

//構造体開放
void turn_angle_structure_free(turn_angle_structure *tas) {
  free(tas);
}

//構造体変数の更新、回転角度の計算
float turn_angle_detection(turn_angle_structure *tas){
  //rotation angle of right wheel
  tas->old_angle = tas->current_angle;

  tas->right_motor_old_angle = tas->right_motor_current_angle;
  tas->right_motor_current_angle = ev3_motor_get_counts(DRIVE_R) - tas->original_right_motor_angle;
  tas->right_motor_rotation_angle =
    tas->right_motor_current_angle - tas->right_motor_old_angle;

  //rotation angle of left wheel
  tas->left_motor_old_angle = tas->left_motor_current_angle;
  tas->left_motor_current_angle = ev3_motor_get_counts(DRIVE_L) - tas->original_left_motor_angle;
  tas->left_motor_rotation_angle =
    tas->left_motor_current_angle - tas->left_motor_old_angle;

  //moving distance and turning angle
  tas->turning_angle = 
	  tas->wheel_across/2 * (tas->right_motor_rotation_angle - tas->left_motor_rotation_angle) 
	  / tas->between_wheels;

  //updating of coordinates
  tas->current_angle = tas->old_angle + tas->turning_angle;

  return tas->current_angle;
}
