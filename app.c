/*
 * ETrikeV firmware
 *
 * app.c for TOPPERS/HRP2
 */

#include "ev3api.h"
#include "app.h"


#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#include "common_definition.h"
#include "Line_tracer.h"
#include "Strategy.h"
#include "Tactics.h"
#include "pid.h"



//戦略の初期化(もろもろやってくれる
//上のSTRATEGYさえ変えれば、該当の戦略に対する戦術がセットされる
Strategy* strategy;

//ライントレースクラスのインスタンス
//各メンバ変数へのset関数を用いて走りをコントロール
//これ以降もset関数で値を各戦略内で自由に変えることが出来る
Line_tracer* line_tracer;





void main_task(intptr_t unused) {
  int grey = (BLACK+WHITE)/2;
  //int light = 0;
  //int count = 0;

  // Configure motors
  ev3_motor_config(STEER, LARGE_MOTOR);
  ev3_motor_config(DRIVE_L, LARGE_MOTOR);
  ev3_motor_config(DRIVE_R, LARGE_MOTOR);

  // Configure sensors
  ev3_sensor_config(IR, COLOR_SENSOR);
  ev3_sensor_config(TOUCH, TOUCH_SENSOR);

  ev3_led_set_color(LED_GREEN);

  ev3_motor_reset_counts(STEER);
  ev3_motor_reset_counts(DRIVE_L);
  ev3_motor_reset_counts(DRIVE_R);

  while(!ev3_touch_sensor_is_pressed(TOUCH));
  tslp_tsk(500); /* 500msec wait */

  char str[20];
  sprintf(str, "%d", 1);
  ev3_lcd_draw_string(str, 0, 0);


  /*
  //戦略の初期化(もろもろやってくれる
  //上のSTRATEGYさえ変えれば、該当の戦略に対する戦術がセットされる
  Strategy* strategy = Strategy_make(STRATEGY);

  //ライントレースクラスのインスタンス
  //各メンバ変数へのset関数を用いて走りをコントロール
  //これ以降もset関数で値を各戦略内で自由に変えることが出来る
  Line_tracer* line_tracer = Line_tracer_make();
  Line_tracer_init_pid(line_tracer, 4.0, 0.2, 0.1, grey);
  Line_tracer_set_current_edge(line_tracer, right);
  Line_tracer_set_target_grey_point(line_tracer, grey);
  Line_tracer_set_forward(line_tracer, 0);
  Line_tracer_set_turn(line_tracer, 0);
  */

  strategy = Strategy_make(STRATEGY);
  line_tracer = Line_tracer_make();
  Line_tracer_init_pid(line_tracer, 4.0, 0.2, 0.1, grey);
  Line_tracer_set_current_edge(line_tracer, right);
  Line_tracer_set_target_grey_point(line_tracer, grey);
  Line_tracer_set_forward(line_tracer, 0);
  Line_tracer_set_turn(line_tracer, 0);

  while(!ev3_button_is_pressed(BACK_BUTTON)) {
    Strategy_update(strategy);
    Line_tracer_output(line_tracer);
    tslp_tsk(10); /* 10msec wait */
    /*
    light = ev3_color_sensor_get_reflect(IR);
    count = ev3_motor_get_counts(STEER);
    if(light>grey){
      if(count<MAX_STEERING_ANGLE){
	ev3_motor_set_power(STEER, 100);
      }else{
	ev3_motor_stop(STEER, true);
      }
      ev3_motor_set_power(DRIVE_L, -DRIVING_POWER);
      ev3_motor_set_power(DRIVE_R,   1);
    }else{
      if(count>-MAX_STEERING_ANGLE){
	ev3_motor_set_power(STEER, -100);
      }else{
	ev3_motor_stop(STEER, true);
      }
      ev3_motor_set_power(DRIVE_L,   1);
      ev3_motor_set_power(DRIVE_R, -DRIVING_POWER);
    */
  }

  ev3_motor_stop(STEER, false);
  ev3_motor_stop(DRIVE_L, false);
  ev3_motor_stop(DRIVE_R, false);

  ext_tsk();
}



