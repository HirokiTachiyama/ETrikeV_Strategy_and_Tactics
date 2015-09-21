#ifndef __STRATEGY__
#define __STRATEGY__

#include "common_definition.h"
#include "Tactics.h"

//戦略名一覧
//戦略は戦術を複数個並べたもの
enum Strategy_name{
  L_ALL,    //Ｌコースをボーナスステージまで全て攻略狙い
  R_ALL,    //Ｒコースをボーナスステージまで全て攻略狙い
  R_U_TURN, //Ｒコースをゴール直後にＵターンする
  TEST_strategy,      //テスト用戦略
  L_Akino               //Lコース、第一ゴールゲートまで
};

typedef struct Strategy_structure{
  //戦略構造体
  //tactics_main         :戦術構造体本体 実際に戦術構造体をここで扱う
  //tactics_current_name :現在の戦術名
  //tactics_name         :戦術名配列     その戦略内で行う戦術名の配列
  //tactics_index        :戦術index      現在行っている戦術のindex
  Tactics* tactics_main;
  enum Tactics_name* tactics_name_array;
  int tactics_current_index;
}Strategy;

Strategy* Strategy_make(enum Strategy_name strategy_name);
void Strategy_set_tactics_array(Strategy* strategy, enum Strategy_name strategy_name);
void Strategy_update(Strategy* strategy);
void Strategy_switch_next_tactics(Strategy* strategy);



#endif


