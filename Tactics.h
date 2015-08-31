#ifndef __TACTICS__
#define __TACTICS__

#include "common_definition.h"
//#include "Strategy.h"

//戦術名一覧
//戦術内には複数のステップが存在する
enum Tactics_name{
  //Rコースの戦術
  R_FIGURE_L,                          //フィギュアL
  R_SHINKANSEN,                        //新幹線
  R_PARALLEL_PARKING,                  //縦列駐車

  //Lコースの戦術
  L_DUAL_BRIDGE,                       //二本橋
  L_BARCODE,                           //バーコード
  L_SPECIFICATIONS_UNDETERMAINED_AREA, //仕様未確定エリア
  L_RIGHT_ANGLE_PARKING,               //直角駐車場

  //汎用
  STANDARD,                            //通常走行
  U_TURN,                              //Uターンを行う
  FINISHED,                            //strategy型の中のtactics_nameの最後に入れる事
  TEST_tactics1,                         //TEST戦略1
  TEST_tactics2                         //TEST戦略2
};


typedef struct Tactics_structure{
  //戦術構造体
  //current_step 今のステップ
  //finish_this_tactics 今の戦術が終わったか否かのフラグ
  int current_step;
  enum Tactics_name tactics_current_name;
  enum yes_or_no finish_this_tactics;
}Tactics;


Tactics* Tactics_make(enum Tactics_name tactics_name);
enum yes_or_no Tactics_is_finished(Tactics* tactics);
void Tactics_update(Tactics* tactics);


//戦術いちらん
void Tactics_TEST1(Tactics* tactics);
void Tactics_TEST2(Tactics* tactics);
void Tactics_FINISHED(Tactics* tactics);


#endif


