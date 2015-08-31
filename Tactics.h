#ifndef __TACTICS__
#define __TACTICS__

#include "common_definition.h"
//#include "Strategy.h"

//��p���ꗗ
//��p���ɂ͕����̃X�e�b�v�����݂���
enum Tactics_name{
  //R�R�[�X�̐�p
  R_FIGURE_L,                          //�t�B�M���AL
  R_SHINKANSEN,                        //�V����
  R_PARALLEL_PARKING,                  //�c�񒓎�

  //L�R�[�X�̐�p
  L_DUAL_BRIDGE,                       //��{��
  L_BARCODE,                           //�o�[�R�[�h
  L_SPECIFICATIONS_UNDETERMAINED_AREA, //�d�l���m��G���A
  L_RIGHT_ANGLE_PARKING,               //���p���ԏ�

  //�ėp
  STANDARD,                            //�ʏ푖�s
  U_TURN,                              //U�^�[�����s��
  FINISHED,                            //strategy�^�̒���tactics_name�̍Ō�ɓ���鎖
  TEST_tactics1,                         //TEST�헪1
  TEST_tactics2                         //TEST�헪2
};


typedef struct Tactics_structure{
  //��p�\����
  //current_step ���̃X�e�b�v
  //finish_this_tactics ���̐�p���I��������ۂ��̃t���O
  int current_step;
  enum Tactics_name tactics_current_name;
  enum yes_or_no finish_this_tactics;
}Tactics;


Tactics* Tactics_make(enum Tactics_name tactics_name);
enum yes_or_no Tactics_is_finished(Tactics* tactics);
void Tactics_update(Tactics* tactics);


//��p�������
void Tactics_TEST1(Tactics* tactics);
void Tactics_TEST2(Tactics* tactics);
void Tactics_FINISHED(Tactics* tactics);


#endif


