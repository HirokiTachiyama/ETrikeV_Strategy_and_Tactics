#include "common_definition.h"
#include "Tactics.h"
#include "Line_tracer.h"





extern Line_tracer* line_tracer;

void Tactics_TEST2(Tactics* tactics){

  char str[20];
  switch(tactics->current_step) {

  case 0:
    ev3_speaker_play_tone(NOTE_B5, 500);
    Line_tracer_set_forward(line_tracer, -50);
    Line_tracer_set_turn(line_tracer, 0);
    line_tracer->run_counter = (ev3_motor_get_counts(DRIVE_L) + ev3_motor_get_counts(DRIVE_R))/2;
    if(line_tracer->run_counter < -300){
      line_tracer->run_counter = 0;
      tactics->current_step++;
    }
    break;

  case 1:
    tactics->finish_this_tactics = yes;
    sprintf(str, "DONE");
    ev3_lcd_draw_string(str, 0, 70);

    break;

  default:
    break;
  }
}
