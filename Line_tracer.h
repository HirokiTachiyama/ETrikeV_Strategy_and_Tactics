#ifndef __LINE_TRACER__
#define __LINE_TRACER__

#include "common_definition.h"
#include "pid.h"
#include "ev3api.h"

typedef struct Line_tracer_structure{
  //signed char forward;   //�O�i���x
  //signed char turn;      //���񑬓x
  signed char forward;   //�O�i���x
  double turn;      //���񑬓x
  int run_counter;       //����Ƃ��Ɏg���J�E���^ �����R�ɂ��g��������
  pid* turn_pid; //turn�pPID
  //Motor_controler* Motor_controler; //���[�^�[����N���X
  enum edge current_edge; //�����̍��E�ǂ��瑤���s�Ȃ̂�
  int target_grey_point; //�����̊���
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

