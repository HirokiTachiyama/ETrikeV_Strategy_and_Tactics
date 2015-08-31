
#include "common_definition.h"
#include "Line_tracer.h"
#include "Strategy.h"
#include "Tactics.h"

Tactics* Tactics_make(enum Tactics_name tactics_name) {
  Tactics* new_tactics = (Tactics*)malloc(sizeof(Tactics));
  new_tactics->tactics_current_name = tactics_name;
  new_tactics->current_step = 0;
  new_tactics->finish_this_tactics = no;
  return new_tactics;
}

//Tactics���I����Ă��邩�ǂ����̔���(�ƌ����Ă��t���O��Ԃ����� �R�[�h�𗝉����₷������)
enum yes_or_no Tactics_is_finished(Tactics* tactics) {
  return tactics->finish_this_tactics;
}

/***

    �������̃R�[�h�ł́A�e��p����forward, turn������,���ۂɃ��[�^�[���o�͂��Ă����̂�Tactics_update������
    ���[�^�[�o�͂���p���ōs���Ă��悢�̂ł͂Ȃ����H

 ***/

void Tactics_update(Tactics* tactics){
  char str[20];

  //���ꂼ��̐�p�ɔ�� ��p�ɔ�񂾂�A�����Ƃ��ēn����tactics��current_step�����ē��삵�Ă���
  //�e��p�̒���forward, turn�l������, ���[�^�[����N���X�ɓn���Ă�������
  switch(tactics->tactics_current_name){
  case TEST_tactics1:
    sprintf(str, "%d", 3);
    ev3_lcd_draw_string(str, 0, 0);
    Tactics_TEST1(tactics);
    break;
  case TEST_tactics2:
    sprintf(str, "%d", 4);
    ev3_lcd_draw_string(str, 0, 0);
    Tactics_TEST2(tactics);
    break;

    /*  case STANDARD:
    Tactics_standard(strategy->tactics_main);
    break;
  case U_TURN:
    Tactics_u_turn(stratgy->tactics_main);
    break;
  case R_FIGURE_L:
    Tactics_r_figure(strategy->tactics_main);
    break;
  case R_SHINKANSEN:
    Tactics_r_shinkansen(strategy->tactics_main);
    break;
  case R_PARALLEL_PARKING:
    Tactics_r_parallel_parking(strategy->tactics_main);
    break;
  case L_DUAL_BRIDGE:
    Tactics_l_dual_bridge(strategy->tactics_main);
    break;
  case L_BARCODE:
    Tactics_l_barcode(strategy->tactics_main);
    break;
  case L_SPECIFICATIONS_UNDETERMAINED_AREA:
    Tactics_l_specifications_undetermained_area(strategy->tactics_main);
    break;
  case L_RIGHT_ANGLE_PARKING:
    Tactics_l_right_angle_parking(strategy->tactics_main);
    break;*/
  case FINISHED:
    Tactics_FINISHED(tactics);
    break;
  default:
    break;
  }

  //�e��p���œn���ꂽforward,turn������Line_trace�N���X�̃��\�b�h�Ń��[�^�[�𓮂���
  //���͂܂������̂�,

}

