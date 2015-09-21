#ifndef __STRATEGY__
#define __STRATEGY__

#include "common_definition.h"
#include "Tactics.h"

//�헪���ꗗ
//�헪�͐�p�𕡐����ׂ�����
enum Strategy_name{
  L_ALL,    //�k�R�[�X���{�[�i�X�X�e�[�W�܂őS�čU���_��
  R_ALL,    //�q�R�[�X���{�[�i�X�X�e�[�W�܂őS�čU���_��
  R_U_TURN, //�q�R�[�X���S�[������ɂt�^�[������
  TEST_strategy,      //�e�X�g�p�헪
  L_Akino               //L�R�[�X�A���S�[���Q�[�g�܂�
};

typedef struct Strategy_structure{
  //�헪�\����
  //tactics_main         :��p�\���̖{�� ���ۂɐ�p�\���̂������ň���
  //tactics_current_name :���݂̐�p��
  //tactics_name         :��p���z��     ���̐헪���ōs����p���̔z��
  //tactics_index        :��pindex      ���ݍs���Ă����p��index
  Tactics* tactics_main;
  enum Tactics_name* tactics_name_array;
  int tactics_current_index;
}Strategy;

Strategy* Strategy_make(enum Strategy_name strategy_name);
void Strategy_set_tactics_array(Strategy* strategy, enum Strategy_name strategy_name);
void Strategy_update(Strategy* strategy);
void Strategy_switch_next_tactics(Strategy* strategy);



#endif


