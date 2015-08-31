#include "common_definition.h"
#include "Tactics.h"
#include "Line_tracer.h"


extern Line_tracer* line_tracer;

void Tactics_TEST1(Tactics* tactics){

  char str[20];
  int i = 0;
  switch(tactics->current_step) {

  case 0:
    Line_tracer_set_forward(line_tracer, 20);
    Line_tracer_set_turn(line_tracer, 0);
    line_tracer->run_counter = (ev3_motor_get_counts(DRIVE_L) + ev3_motor_get_counts(DRIVE_R))/2;
    sprintf(str, "count:%d", line_tracer->run_counter);
    ev3_lcd_draw_string(str, 0, 40);
    if(line_tracer->run_counter < -300){
      line_tracer->run_counter = 0;
      tactics->current_step++;
    }
    break;

  case 1:
    Line_tracer_set_forward(line_tracer, 100);
    Line_tracer_set_turn(line_tracer, 50);
    line_tracer->run_counter = (ev3_motor_get_counts(DRIVE_L) + ev3_motor_get_counts(DRIVE_R))/2;
    if(line_tracer->run_counter < -3000){
      line_tracer->run_counter = 0;
      tactics->current_step++;
    }
    break;

  case 2:
    tactics->finish_this_tactics = yes;
    break;

  default:
    break;
  }

}







