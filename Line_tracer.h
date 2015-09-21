#ifndef __LINE_TRACER__
#define __LINE_TRACER__

#include "common_definition.h"
#include "pid.h"
#include "ev3api.h"

typedef struct Line_tracer_structure{
  //signed char forward;   //前進速度
  //signed char turn;      //旋回速度
  signed char forward;   //前進速度
  double turn;      //旋回速度
  int run_counter;       //走るときに使うカウンタ ご自由にお使い下さい
  pid* turn_pid; //turn用PID
  //Motor_controler* Motor_controler; //モーター制御クラス
  enum edge current_edge; //黒線の左右どちら側走行なのか
  int target_grey_point; //白黒の割合
}Line_tracer;

Line_tracer* Line_tracer_make();
void Line_tracer_init_pid(Line_tracer* line_tracer,
			  double p_gain,double i_gain, double d_gain,double target);
void Line_tracer_set_current_edge(Line_tracer* line_tracer, enum edge edge);
void Line_tracer_set_forward(Line_tracer* line_tracer, int forward);
void Line_tracer_set_turn(Line_tracer* line_tracer, int turn);
void Line_tracer_set_target_grey_point(Line_tracer* line_tracer, int target);
void Line_tracer_set_turn_from_pid(Line_tracer* line_tracer);
void Line_tracer_output(Line_tracer* line_tracer);




#endif

