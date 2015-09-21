/*
 * ETrikeV firmware
 *
 * app.c for TOPPERS/HRP2
 */

#include "ev3api.h"
#include "app.h"
#include "pid.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif


#define BLACK  2
#define WHITE 47
#define MAX_STEERING_ANGLE	360
#define DRIVING_POWER		 30
#define DIAMETER 266.9

#define STEER	EV3_PORT_C
#define DRIVE_L	EV3_PORT_A
#define DRIVE_R	EV3_PORT_B
#define IR		EV3_PORT_3
#define TOUCH	EV3_PORT_2
#define GYRO	EV3_PORT_4

#define GYRO_OFFSET 0

#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

#define P_GAIN             2.5F
#define PWM_ABS_MAX          60

int changeStyle = 0;
signed char forward;
signed char turn;

int16_t gyro_before = 0;

int tail_run(int timeCounter){//ウィリー走行
	if(timeCounter <= 17){
		ev3_motor_set_power(DRIVE_L,100);
		ev3_motor_set_power(DRIVE_R,100);
	}else if(timeCounter > 17 && timeCounter <= 37){
		ev3_motor_set_power(DRIVE_L,-100);
		ev3_motor_set_power(DRIVE_R,-100);
	}else if(timeCounter > 37 && timeCounter <= 80){
		ev3_motor_set_power(DRIVE_L,-115 + timeCounter);
		ev3_motor_set_power(DRIVE_R,-115 + timeCounter);
	}else if(timeCounter >= 80){
		ev3_motor_set_power(DRIVE_L,0);
		ev3_motor_set_power(DRIVE_R,0);
		return 1;
	}
	return 0;
}

int bridge(int timeCounter){//二本橋クリア
	ev3_motor_set_power(STEER,0);
	if(timeCounter <= 15){
			ev3_motor_set_power(DRIVE_L,60);
			ev3_motor_set_power(DRIVE_R,60);
	}else if(timeCounter > 15 && timeCounter <= 35){
			ev3_motor_set_power(DRIVE_L,-70);
			ev3_motor_set_power(DRIVE_R,-70);
	}else if(timeCounter > 35 && timeCounter <= 80){
			ev3_motor_set_power(DRIVE_L,-70 + timeCounter/3);
			ev3_motor_set_power(DRIVE_R,-70 + timeCounter/3);
	}else if(timeCounter > 80 && timeCounter <= 130){
			ev3_motor_set_power(DRIVE_L,0);
			ev3_motor_set_power(DRIVE_R,0);
	}else if(timeCounter > 130 && timeCounter < 380){
			ev3_motor_set_power(DRIVE_L,-20);
			ev3_motor_set_power(DRIVE_R,-20);
	}else if(timeCounter >= 380 ){
		ev3_motor_set_power(DRIVE_L,0);
		ev3_motor_set_power(DRIVE_R,0);
		return 1;
	}
	return 0;
}

int stepDetective(int value){//段差検知
	int16_t gyro_after = ev3_gyro_sensor_get_rate(GYRO);
	if(gyro_before - gyro_after >= value || gyro_before - gyro_after <= -value){
			return 1;
	}else{
	 		gyro_before = gyro_after;
			return 0;
	}
}


int atack(int timeCounter){
		ev3_motor_set_power(STEER,0);
		if(ev3_color_sensor_get_color(IR) != 6){
				ev3_motor_set_power(DRIVE_L,-10);
				ev3_motor_set_power(DRIVE_R,-10);
		}else{
				ev3_motor_set_power(DRIVE_L,0);
				ev3_motor_set_power(DRIVE_R,0);
			return 1;
		}
		return 0;
}


void main_task(intptr_t unused) {
	int grey = (BLACK+WHITE)/2;
	int white_side = (BLACK+WHITE*2)/3;
	int black_side = (BLACK*3+WHITE)/4;
	int light = 0;
	int count = 0;
	unsigned int timeCounter = 1;
	int flag = 0;
	float turn_angle = 0.0;
	int distace = 0;
	int dflag = 0;

	pid *my_pid = pid_make(4.0, 0.2, 0.1, grey); //default pid 1.0 0.02 0.0
	pid *white_pid = pid_make(4.0, 0.2, 0.1, white_side);
	pid *black_pid = pid_make(4.0, 0.2, 0.1, black_side);

	turn_angle_structure *tas = turn_angle_constructor();

	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("EV3way ETRobcon 2015 KatLab", 0, CALIB_FONT_HEIGHT*1);
	ev3_lcd_draw_string("Ver.20150508", 0, CALIB_FONT_HEIGHT*2);
	ev3_lcd_draw_string("right side", 0, CALIB_FONT_HEIGHT*3);
    // Configure motors
  ev3_motor_config(STEER, LARGE_MOTOR);
  ev3_motor_config(DRIVE_L, LARGE_MOTOR);
  ev3_motor_config(DRIVE_R, LARGE_MOTOR);

    // Configure sensors
  ev3_sensor_config(IR, COLOR_SENSOR);
  ev3_sensor_config(TOUCH, TOUCH_SENSOR);
	ev3_sensor_config(GYRO, GYRO_SENSOR);

  ev3_led_set_color(LED_GREEN);

	ev3_motor_reset_counts(STEER);
	ev3_motor_reset_counts(DRIVE_L);
	ev3_motor_reset_counts(DRIVE_R);


	ev3_motor_set_power(STEER,0);


	ev3_gyro_sensor_reset(GYRO);

	while(!ev3_touch_sensor_is_pressed(TOUCH)){
	  //tslp_tsk(500); /* 500msec wait */
	}
	while(!ev3_button_is_pressed(BACK_BUTTON)){
	  light = ev3_color_sensor_get_reflect(IR);
	  count = ev3_motor_get_counts(STEER);

	  switch(changeStyle){
	  case 0://始めのカーブ差し掛かりまで
	    count = ev3_motor_get_counts(STEER);
	    forward = 55;
	    pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
	    turn = (signed char) pid_get_output(*my_pid);

	    if(turn > 100){
	      turn = 100;
	    }else if(turn < -100){
	      turn = -100;
	    }

	    turn = -turn;

	    if(count<MAX_STEERING_ANGLE && count>-MAX_STEERING_ANGLE)
	      {
		ev3_motor_set_power(STEER, turn/2);
	      }else if(count<=-MAX_STEERING_ANGLE && turn>=0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else if(count>=MAX_STEERING_ANGLE && turn<0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else{
	      ev3_motor_stop(STEER, true);
	    }


	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < (-7300 + 2400)){
	      ev3_lcd_draw_string("changeStyle 1", 0, 70);
	      changeStyle = 1;
	    }

	    break;
	  case 1: //カーブ白より走行
	    count = ev3_motor_get_counts(STEER);
	    forward = 26;
	    pid_input(white_pid, ev3_color_sensor_get_reflect(IR));
	    turn = (signed char) pid_get_output(*white_pid);

	    if(turn > 0){
	      turn = turn*2;
	    }
	    if(turn > 100){
	      turn = 100;
	    }else if(turn < -100){
	      turn = -100;
	    }

	    turn = -turn;

	    if(count<MAX_STEERING_ANGLE && count>-MAX_STEERING_ANGLE)
	      {
		ev3_motor_set_power(STEER, turn/2);
	      }else if(count<=-MAX_STEERING_ANGLE && turn>=0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else if(count>=MAX_STEERING_ANGLE && turn<0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else{
	      ev3_motor_stop(STEER, true);
	    }


	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < (-9200 + 2400)){
	      changeStyle = 2;
	    }


	    break;
	  case 2://段差検知まで 黒より走行
	    count = ev3_motor_get_counts(STEER);
	    forward = 30;
	    pid_input(black_pid, ev3_color_sensor_get_reflect(IR));
	    turn = (signed char) pid_get_output(*black_pid);

	    if(turn < 0){
	      turn = turn*2;
	    }
	    if(turn > 100){
	      turn = 100;
	    }else if(turn < -100){
	      turn = -100;
	    }

	    turn = -turn;

	    if(count<MAX_STEERING_ANGLE && count>-MAX_STEERING_ANGLE)
	      {
		ev3_motor_set_power(STEER, turn/2);
	      }else if(count<=-MAX_STEERING_ANGLE && turn>=0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else if(count>=MAX_STEERING_ANGLE && turn<0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else{
	      ev3_motor_stop(STEER, true);
	    }


	    if(stepDetective(12)==1){
	      changeStyle = 3;
	      ev3_lcd_draw_string("changeStyle Now", 0, 70);
	      flag = 1;
	    }

	    break;
	  case 3://二本橋クリアする
	    if(bridge(timeCounter) == 1){
	      changeStyle = 13;
	    }
	    timeCounter++;
	    break;
	  case 4://止まる
	    if(ev3_color_sensor_get_color(IR) == 6 || ev3_color_sensor_get_color(IR) == 3){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	    }else{
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,-20);
	      ev3_motor_set_power(DRIVE_R,-20);
	    }
	    break;

	  case 5://バーコード終了後ライントレース
	    count = ev3_motor_get_counts(STEER);
	    forward = 25;
	    pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
	    turn = (signed char) pid_get_output(*my_pid);

	    if(turn > 100){
	      turn = 100;
	    }else if(turn < -100){
	      turn = -100;
	    }

	    turn = -turn;

	    if(count<MAX_STEERING_ANGLE && count>-MAX_STEERING_ANGLE)
	      {
		ev3_motor_set_power(STEER, turn/2);
	      }else if(count<=-MAX_STEERING_ANGLE && turn>=0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else if(count>=MAX_STEERING_ANGLE && turn<0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else{
	      ev3_motor_stop(STEER, true);
	    }

	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -300){
	      changeStyle = 6;
	      flag = 1;
	      ev3_motor_reset_counts(DRIVE_L);
	      ev3_motor_reset_counts(DRIVE_R);
	    }
	    break;
	  case 6://まっすぐ走る
	    ev3_motor_set_power(STEER,0);
	    ev3_motor_set_power(DRIVE_L,-40);
	    ev3_motor_set_power(DRIVE_R,-40);
	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -750){
	      changeStyle = 7;
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	      timeCounter = 0;
	    }
	    break;

	  case 7://前輪を真横にする
	    if(timeCounter <= 9){
	      ev3_motor_rotate(STEER,90,50,true);
	    }else if(timeCounter > 9){
	      changeStyle = 8;
	      ev3_lcd_draw_string("changeStyle Now", 0, 70);
	      timeCounter = 0;
	    }
	    timeCounter++;
	    break;
	  case 8://車体を左向きに
	    turn_angle = turn_angle_detection(tas);
	    if(turn_angle >= -76.0){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,30);
	      ev3_motor_set_power(DRIVE_R,-20);
	    }else if(turn_angle < -76.0){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	      changeStyle = 9;
	    }
	    break;
	  case 9://前輪を戻す
	    ev3_motor_rotate(STEER,-900,50,true);
	    changeStyle = 10;
	    ev3_lcd_draw_string("changeStyle Now", 0, 70);
	    ev3_motor_reset_counts(DRIVE_L);
	    ev3_motor_reset_counts(DRIVE_R);
	    break;

	  case 10://使用未確定エリアに乗る
	    ev3_motor_set_power(DRIVE_L,-50);
	    ev3_motor_set_power(DRIVE_R,-50);
	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -500){
	      timeCounter++;
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	    }
	    if(timeCounter >= 100){
	      timeCounter=0;
	      changeStyle=11;
	    }
	    break;
	  case 11://ウィリー走行に切り替え
	    if(tail_run(timeCounter) == 1){
	      changeStyle = 12;
	      timeCounter = 0;
	    }else{
	      timeCounter++;
	    }

	    break;

	  case 12://止まる
	    ev3_motor_set_power(STEER,0);
	    ev3_motor_set_power(DRIVE_L,0);
	    ev3_motor_set_power(DRIVE_R,0);
	    break;

	  case 13://ライン復帰
	    ev3_motor_rotate(STEER,940,50,true);
	    changeStyle = 14;
	    ev3_lcd_draw_string("changeStyle Now", 0, 70);
	    timeCounter = 1;
	    break;
	  case 14:
	    if(timeCounter < 37){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,30);
	      ev3_motor_set_power(DRIVE_R,-30);
	    }else if(timeCounter >= 37){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	      changeStyle = 15;
	      timeCounter = 0;
	    }timeCounter++;
	    break;
	  case 15:
	    ev3_motor_rotate(STEER,-1000,50,true);
	    changeStyle = 16;
	    ev3_lcd_draw_string("changeStyle Now", 0, 70);
	    break;
	  case 16:

	    ev3_motor_set_power(STEER,0);
	    if(timeCounter <= 20){
	      ev3_motor_set_power(DRIVE_L,15);
	      ev3_motor_set_power(DRIVE_R,15);
	    }else if(timeCounter > 20 && (timeCounter -20) /60 % 2 == 1){
	      ev3_motor_set_power(DRIVE_L,15);
	      ev3_motor_set_power(DRIVE_R,15);
	    }else if(timeCounter > 20 && (timeCounter - 20) /60 % 2 == 0){
	      ev3_motor_set_power(DRIVE_L,-15);
	      ev3_motor_set_power(DRIVE_R,-15);
	    }
	    timeCounter++;
	    if(ev3_color_sensor_get_color(IR) == 1){
	      changeStyle = 30;
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	      timeCounter = 0;
	    }
	    break;
	  case 17:
	    ev3_motor_rotate(STEER,830,50,true);
	    changeStyle = 18;
	    ev3_lcd_draw_string("changeStyle Now", 0, 70);
	    break;
	  case 18:
	    if(ev3_color_sensor_get_reflect(IR) > white_side){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,-6);
	      ev3_motor_set_power(DRIVE_R,6);
	    }else if(ev3_color_sensor_get_reflect(IR) < white_side){
	      ev3_motor_set_power(STEER,0);
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	      changeStyle = 19;
	    }
	    break;
	  case 19:
	    ev3_motor_rotate(STEER,-950,40,true);
	    changeStyle = 20;
	    ev3_lcd_draw_string("changeStyle Now", 0, 70);
	    break;
	  case 20:
	    ev3_motor_set_power(STEER,0);
	    ev3_motor_set_power(DRIVE_L,-17);
	    ev3_motor_set_power(DRIVE_R,-15);
	    if(ev3_color_sensor_get_color(IR) == 1){
	      changeStyle = 21;
	      flag = 0;
	      ev3_motor_reset_counts(DRIVE_L);
	      ev3_motor_reset_counts(DRIVE_R);
	    }
	    break;
	  case 21:
	    count = ev3_motor_get_counts(STEER);
	    forward = 14;
	    pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
	    turn = (signed char) pid_get_output(*my_pid);

	    if(turn > 100){
	      turn = 100;
	    }else if(turn < -100){
	      turn = -100;
	    }


	    if(count<MAX_STEERING_ANGLE && count>-MAX_STEERING_ANGLE)
	      {
		ev3_motor_set_power(STEER, turn/2);
	      }else if(count<=-MAX_STEERING_ANGLE && turn>=0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else if(count>=MAX_STEERING_ANGLE && turn<0){
	      ev3_motor_set_power(STEER, turn/2);
	    }else{
	      ev3_motor_stop(STEER, true);
	    }

	    if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -3000){
	      changeStyle = 5;
	      ev3_motor_reset_counts(DRIVE_L);
	      ev3_motor_reset_counts(DRIVE_R);
	    }

	    break;
	  case 30:
	    ev3_motor_set_power(STEER,0);
	    if(timeCounter <= 65){
	      ev3_motor_set_power(DRIVE_L,-17);
	      ev3_motor_set_power(DRIVE_R,-17);
	    }else if(timeCounter > 65 && timeCounter <= 120){
	      ev3_motor_set_power(DRIVE_L,0);
	      ev3_motor_set_power(DRIVE_R,0);
	    }else if(timeCounter > 120){
	      changeStyle = 17;
	      timeCounter = 0;
	    }			timeCounter++;
	  default:

	    break;
	  }
	  if(flag==0){
	    ev3_motor_steer(DRIVE_L, DRIVE_R, -forward, -turn);
	  }

	  tslp_tsk(10); /* 10msec wait */

	}

	ev3_motor_stop(STEER, false);
	ev3_motor_stop(DRIVE_L, false);
	ev3_motor_stop(DRIVE_R, false);

	ext_tsk();
}
