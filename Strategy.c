#include "common_definition.h"
#include "Strategy.h"
#include "Tactics.h"

Strategy* Strategy_make(enum Strategy_name strategy_name){
  Strategy* new_strategy = (Strategy*)malloc(sizeof(Strategy));
  int tactics_num = FINISHED; //������ѿ���enum tactics_name�κǸ��FINISHED(=���)
  new_strategy->tactics_name_array =
    (enum Tactics_name*)malloc( sizeof(enum Tactics_name) * tactics_num); //���̾����Υ������
  new_strategy->tactics_current_index = 0; //��Ѥ�index��0��
  Strategy_set_tactics_array(new_strategy, strategy_name); //���̾��������̾�򥻥å�
  new_strategy->tactics_main = Tactics_make(new_strategy->tactics_name_array[0]);
  return new_strategy;
}

void Strategy_set_tactics_array(Strategy* strategy, enum Strategy_name strategy_name){
  switch(strategy_name) {
  case L_ALL: //��ά���̥�����������������
    strategy->tactics_name_array[0] = STANDARD;                            //�������ȏ�����ܶ�
    strategy->tactics_name_array[1] = L_DUAL_BRIDGE;                       //���ܶ�
    strategy->tactics_name_array[2] = L_BARCODE;                           //�С�������
    strategy->tactics_name_array[3] = L_SPECIFICATIONS_UNDETERMAINED_AREA; //����̤���ꥨ�ꥢ
    strategy->tactics_name_array[4] = STANDARD;                            //����̤���ꥨ�ꥢ�叢�������
    strategy->tactics_name_array[5] = L_RIGHT_ANGLE_PARKING;               //ľ����־�
    strategy->tactics_name_array[6] = FINISHED;                            //�����ޤ�
    break;

  case R_ALL: //��ά���ҥ�����������������
    strategy->tactics_name_array[0] = STANDARD;           //�������ȏ���ե����奢����
    strategy->tactics_name_array[1] = R_FIGURE_L;         //�ե����奢��
    strategy->tactics_name_array[2] = R_SHINKANSEN;       //������
    strategy->tactics_name_array[3] = STANDARD;           //�������叢����󥴡����
    strategy->tactics_name_array[4] = R_PARALLEL_PARKING; //�������
    strategy->tactics_name_array[5] = FINISHED;           //�����ޤ�
    break;

  case R_U_TURN: //��ά���ҥ�������쥴�����̲��ˣե�����
    strategy->tactics_name_array[0] = STANDARD;           //�������ȏ����쥴���륲���ȣ�
    strategy->tactics_name_array[1] = U_TURN;             //�ե�����
    strategy->tactics_name_array[2] = STANDARD;           //��쥴���륲���ȣҏ����������
    strategy->tactics_name_array[3] = R_PARALLEL_PARKING; //�������
    strategy->tactics_name_array[4] = FINISHED;           //�����ޤ�
    break;
  case TEST_strategy:
    strategy->tactics_name_array[0] = TEST_tactics1;
    strategy->tactics_name_array[1] = TEST_tactics2;
    strategy->tactics_name_array[2] = FINISHED;
    break;
  case L_Akino:
    strategy->tactics_name_array[0] = L_Akino_tactics;
    strategy->tactics_name_array[1] = FINISHED;
    break;

  default:
    break;
  }
}

void Strategy_update(Strategy* strategy){
  //  char str[20];
  //  sprintf(str, "%d", 2);
  //  ev3_lcd_draw_string(str, 0, 10);
  //�⤷���ߤ���Ѥ�����äƤ����鼡����Ѥذܹ�
  if(Tactics_is_finished(strategy->tactics_main) == yes)
    Strategy_switch_next_tactics(strategy);
  //���ߤ���Ѥ򼡤Υ��ƥåפؿʤ��
  Tactics_update(strategy->tactics_main);
}

void Strategy_switch_next_tactics(Strategy* strategy){
  //�����Σ��ĤΥ����ѿ��򹹿�����
  strategy->tactics_current_index++; //����ǥå����򼡤˿ʤ��
  strategy->tactics_main =
    Tactics_make(strategy->tactics_name_array[strategy->tactics_current_index]);//���Υ���ǥå�������Ѥ�make
  strategy->tactics_main->tactics_current_name = strategy->tactics_name_array[strategy->tactics_current_index]; //����Τ��Υ���ǥå�����̾���򸽺ߤ����̾��
  strategy->tactics_main->finish_this_tactics = no; //��ѽ�λ�ե饰��no��
}



