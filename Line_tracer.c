#include "common_definition.h"
#include "Line_tracer.h"

Line_tracer* Line_tracer_make(){
  Line_tracer* new_line_tracer = (Line_tracer*)malloc(sizeof(Line_tracer));
  new_line_tracer->forward = 0;
  new_line_tracer->turn = 0;
  new_line_tracer->run_counter = 0;
  new_line_tracer->current_edge = right;
  new_line_tracer->target_grey_point = 0;
  return new_line_tracer;
}

void Line_tracer_init_pid(Line_tracer* line_tracer,
			 double p_gain,double i_gain, double d_gain,double target){
  line_tracer->turn_pid = pid_make(p_gain, i_gain, d_gain, target);
}

void Line_tracer_set_current_edge(Line_tracer* line_tracer, enum edge edge){
  line_tracer->current_edge = edge;
}
void Line_tracer_set_forward(Line_tracer* line_tracer, int forward){
  line_tracer->forward = forward;
}

void Line_tracer_set_turn(Line_tracer* line_tracer, int turn){
  line_tracer->turn = turn;
}
void Line_tracer_set_target_grey_point(Line_tracer* line_tracer, int target){
  line_tracer->target_grey_point = target;
}
//PIDからturnを算出して、Line_tracerのメンバ変数turnにセット
void Line_tracer_set_turn_from_pid(Line_tracer* line_tracer){
  pid_input(line_tracer->turn_pid, ev3_color_sensor_get_reflect(IR));
  line_tracer->turn = pid_get_output(line_tracer->turn_pid);
  if(line_tracer->turn > 100)       line_tracer->turn = 100;
  else if(line_tracer->turn < -100) line_tracer->turn = -100;
}

void Line_tracer_output(Line_tracer* line_tracer){
  int steer_count = ev3_motor_get_counts(STEER);
  //STEER制御
  if(steer_count<MAX_STEERING_ANGLE && steer_count>-MAX_STEERING_ANGLE) {
    ev3_motor_set_power(STEER, line_tracer->turn/2);
  }else if(steer_count<=-MAX_STEERING_ANGLE &&  line_tracer->turn>=0){
    ev3_motor_set_power(STEER,  line_tracer->turn/2);
  }else if(steer_count>=MAX_STEERING_ANGLE &&  line_tracer->turn<0){
    ev3_motor_set_power(STEER,  line_tracer->turn/2);
  }else{
    ev3_motor_stop(STEER, true);
  }
  //後輪制御
  ev3_motor_steer(DRIVE_L, DRIVE_R, -line_tracer->forward, - line_tracer->turn);
}

