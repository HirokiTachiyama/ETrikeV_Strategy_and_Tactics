#include "common_definition.h"
#include "Strategy.h"
#include "Tactics.h"

Strategy* Strategy_make(enum Strategy_name strategy_name){
  Strategy* new_strategy = (Strategy*)malloc(sizeof(Strategy));
  int tactics_num = FINISHED; //最大戦術数はenum tactics_nameの最後のFINISHED(=定数)
  new_strategy->tactics_name_array =
    (enum Tactics_name*)malloc( sizeof(enum Tactics_name) * tactics_num); //戦術名配列のメモリ確保
  new_strategy->tactics_current_index = 0; //戦術のindexを0に
  Strategy_set_tactics_array(new_strategy, strategy_name); //戦術名配列に戦術名をセット
  new_strategy->tactics_main = Tactics_make(new_strategy->tactics_name_array[0]);
  return new_strategy;
}

void Strategy_set_tactics_array(Strategy* strategy, enum Strategy_name strategy_name){
  switch(strategy_name) {
  case L_ALL: //戦略：Ｌコース完全走破狙い
    strategy->tactics_name_array[0] = STANDARD;                            //スタート~二本橋
    strategy->tactics_name_array[1] = L_DUAL_BRIDGE;                       //二本橋
    strategy->tactics_name_array[2] = L_BARCODE;                           //バーコード
    strategy->tactics_name_array[3] = L_SPECIFICATIONS_UNDETERMAINED_AREA; //仕様未確定エリア
    strategy->tactics_name_array[4] = STANDARD;                            //仕様未確定エリア後~ゴール
    strategy->tactics_name_array[5] = L_RIGHT_ANGLE_PARKING;               //直角駐車場
    strategy->tactics_name_array[6] = FINISHED;                            //おしまい
    break;

  case R_ALL: //戦略：Ｒコース完全走破狙い
    strategy->tactics_name_array[0] = STANDARD;           //スタート~フィギュアＬ前
    strategy->tactics_name_array[1] = R_FIGURE_L;         //フィギュアＬ
    strategy->tactics_name_array[2] = R_SHINKANSEN;       //新幹線
    strategy->tactics_name_array[3] = STANDARD;           //新幹線後~第二ゴールＲ
    strategy->tactics_name_array[4] = R_PARALLEL_PARKING; //縦列駐車
    strategy->tactics_name_array[5] = FINISHED;           //おしまい
    break;

  case R_U_TURN: //戦略：Ｒコース第一ゴール通過後にＵターン
    strategy->tactics_name_array[0] = STANDARD;           //スタート~第一ゴールゲートＲ
    strategy->tactics_name_array[1] = U_TURN;             //Ｕターン
    strategy->tactics_name_array[2] = STANDARD;           //第一ゴールゲートＲ~スタート
    strategy->tactics_name_array[3] = R_PARALLEL_PARKING; //縦列駐車
    strategy->tactics_name_array[4] = FINISHED;           //おしまい
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
  //もし現在の戦術が終わっていたら次の戦術へ移行
  if(Tactics_is_finished(strategy->tactics_main) == yes)
    Strategy_switch_next_tactics(strategy);
  //現在の戦術を次のステップへ進める
  Tactics_update(strategy->tactics_main);
}

void Strategy_switch_next_tactics(Strategy* strategy){
  //戦術中の３つのメンバ変数を更新する
  strategy->tactics_current_index++; //インデックスを次に進めて
  strategy->tactics_main =
    Tactics_make(strategy->tactics_name_array[strategy->tactics_current_index]);//そのインデックスの戦術をmake
  strategy->tactics_main->tactics_current_name = strategy->tactics_name_array[strategy->tactics_current_index]; //配列のそのインデックスの名前を現在の戦術名に
  strategy->tactics_main->finish_this_tactics = no; //戦術終了フラグをnoに
}



