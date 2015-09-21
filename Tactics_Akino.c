#include "common_definition.h"
#include "Tactics.h"
#include "Line_tracer.h"


signed char forward;
signed char turn;
int changeStyle = 0;
int16_t gyro_before = 0;
int grey;
int white_side;
int black_side;
int light = 0;
int count = 0;
unsigned int timeCounter = 1;
int flag = 0;
float turn_angle = 0.0;
int distance = 0;
int dflag = 0;
pid *my_pid;
pid *white_pid;
pid *black_pid;
turn_angle_structure *tas;


extern Line_tracer* line_tracer;


//コンストラクタ
turn_angle_structure* turn_angle_constructor(){
  //setting sensor value of radius to 0
  ev3_motor_reset_counts(DRIVE_R); //right motor
  ev3_motor_reset_counts(DRIVE_L); //left motor
  return turn_angle_structure_make();
}

//回転角度検知用の構造体作成
turn_angle_structure *turn_angle_structure_make(void) {
  turn_angle_structure *new_turn_angle_structure =
    (turn_angle_structure*)malloc(sizeof(turn_angle_structure));
  new_turn_angle_structure->right_motor_current_angle = ev3_motor_get_counts(DRIVE_R);
  new_turn_angle_structure->left_motor_current_angle = ev3_motor_get_counts(DRIVE_L);
  new_turn_angle_structure->right_motor_old_angle = 0;
  new_turn_angle_structure->left_motor_old_angle  = 0;
  new_turn_angle_structure->right_motor_rotation_angle = 0;
  new_turn_angle_structure->left_motor_rotation_angle = 0;
  new_turn_angle_structure->wheel_across = 8.0;
  new_turn_angle_structure->between_wheels = 12.0;
  new_turn_angle_structure->moving_distance = 0;
  new_turn_angle_structure->turning_angle = 0;
  new_turn_angle_structure->old_angle = 0;
  new_turn_angle_structure->current_angle = 0;
  new_turn_angle_structure->original_right_motor_angle = 0;
  new_turn_angle_structure->original_left_motor_angle = 0;
  return new_turn_angle_structure;
}

//構造体開放
void turn_angle_structure_free(turn_angle_structure *tas) {
  free(tas);
}

//構造体変数の更新、回転角度の計算
float turn_angle_detection(turn_angle_structure *tas){
  //rotation angle of right wheel
  tas->old_angle = tas->current_angle;

  tas->right_motor_old_angle = tas->right_motor_current_angle;
  tas->right_motor_current_angle = ev3_motor_get_counts(DRIVE_R) - tas->original_right_motor_angle;
  tas->right_motor_rotation_angle =
    tas->right_motor_current_angle - tas->right_motor_old_angle;

  //rotation angle of left wheel
  tas->left_motor_old_angle = tas->left_motor_current_angle;
  tas->left_motor_current_angle = ev3_motor_get_counts(DRIVE_L) - tas->original_left_motor_angle;
  tas->left_motor_rotation_angle =
    tas->left_motor_current_angle - tas->left_motor_old_angle;

  //moving distance and turning angle
  tas->turning_angle = 
    tas->wheel_across/2 * (tas->right_motor_rotation_angle - tas->left_motor_rotation_angle) 
    / tas->between_wheels;

  //updating of coordinates
  tas->current_angle = tas->old_angle + tas->turning_angle;

  return tas->current_angle;
}

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


/*
 *変更点
 *初めの変数初期化を外に
 *changeStyle を tactics->current_step
 *case 0に初めの初期化処理を当てはめたので、一つづつずらした
 */



void Tactics_Akino(Tactics* tactics) {

  switch(tactics->current_step){

  case 0:
    ev3_speaker_play_tone(NOTE_C5, 20);

    //    Line_tracer_set_forward

    changeStyle=0;
    gyro_before=0;
    grey=(BLACK+WHITE)/2;
    white_side=(BLACK+WHITE*2)/3;
    black_side=(BLACK*2+WHITE)/3;
    light=0;
    count=0;
    timeCounter=1;
    flag=0;
    turn_angle=0.0;
    distance=0;
    dflag=0;
    my_pid= pid_make(4.0, 0.2, 0.1, grey); //default pid 1.0 0.02 0.0
    white_pid = pid_make(4.0, 0.2, 0.1, white_side);
    black_pid = pid_make(4.0, 0.2, 0.1, black_side);
    tas = turn_angle_constructor();

    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("EV3way ETRobcon 2015 KatLab", 0, CALIB_FONT_HEIGHT*1);
    ev3_lcd_draw_string("Ver.20150508", 0, CALIB_FONT_HEIGHT*2);
    ev3_lcd_draw_string("right side", 0, CALIB_FONT_HEIGHT*3);
    // Configure motors

    tactics->current_step++;
    tslp_tsk(500); /* 500msec wait */
    break;


  case 1://始めのカーブ差し掛かりまで
    light = ev3_color_sensor_get_reflect(IR);

    count = ev3_motor_get_counts(STEER);
    forward = 43;
    pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
    turn = (signed char) pid_get_output(my_pid);

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
      ev3_speaker_play_tone(NOTE_D5, 20);
      ev3_lcd_draw_string("tactics->current_step 1", 0, 70);
      tactics->current_step = 2;
    }
    break;

  case 2: //カーブ白より走行
      count = ev3_motor_get_counts(STEER);
      forward = 26;
      pid_input(white_pid, ev3_color_sensor_get_reflect(IR));
      turn = (signed char) pid_get_output(white_pid);

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
	ev3_speaker_play_tone(NOTE_E5, 20);
	tactics->current_step = 3;
      }


      break;
    case 3://段差検知まで 黒より走行
      count = ev3_motor_get_counts(STEER);
      forward = 28;
      pid_input(black_pid, ev3_color_sensor_get_reflect(IR));
      turn = (signed char) pid_get_output(black_pid);

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
	tactics->current_step = 4;
	ev3_lcd_draw_string("tactics->current_step Now3", 0, 70);
	flag = 1;
      }

      break;
    case 4://二本橋クリアする
      if(bridge(timeCounter) == 1){
	tactics->current_step = 14;
      }
      timeCounter++;
      break;
    case 5://止まる
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

    case 6://バーコード終了後ライントレース
      count = ev3_motor_get_counts(STEER);
      forward = 25;
      pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
      turn = (signed char) pid_get_output(my_pid);

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
	tactics->current_step = 6;
	flag = 1;
	ev3_motor_reset_counts(DRIVE_L);
	ev3_motor_reset_counts(DRIVE_R);
      }
      break;
    case 7://まっすぐ走る
      ev3_motor_set_power(STEER,0);
      ev3_motor_set_power(DRIVE_L,-40);
      ev3_motor_set_power(DRIVE_R,-40);
      if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -750){
	tactics->current_step = 7;
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
	timeCounter = 0;
      }
      break;

    case 8://前輪を真横にする
      if(timeCounter <= 9){
	ev3_motor_rotate(STEER,90,50,true);
      }else if(timeCounter > 9){
	tactics->current_step = 8;
	ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
	timeCounter = 0;
      }
      timeCounter++;
      break;
    case 9://車体を左向きに
      turn_angle = turn_angle_detection(tas);
      if(turn_angle >= -76.0){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,30);
	ev3_motor_set_power(DRIVE_R,-20);
      }else if(turn_angle < -76.0){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
	tactics->current_step = 9;
      }
      break;
    case 10://前輪を戻す
      ev3_motor_rotate(STEER,-900,50,true);
      tactics->current_step = 10;
      ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
      ev3_motor_reset_counts(DRIVE_L);
      ev3_motor_reset_counts(DRIVE_R);
      break;

    case 11://使用未確定エリアに乗る
      ev3_motor_set_power(DRIVE_L,-50);
      ev3_motor_set_power(DRIVE_R,-50);
      if((ev3_motor_get_counts(DRIVE_L)+ev3_motor_get_counts(DRIVE_R))/2 < -500){
	timeCounter++;
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
      }
      if(timeCounter >= 100){
	timeCounter=0;
	tactics->current_step=11;
      }
      break;
    case 12://ウィリー走行に切り替え
      if(tail_run(timeCounter) == 1){
	tactics->current_step = 12;
	timeCounter = 0;
      }else{
	timeCounter++;
      }

      break;

    case 13://止まる
      ev3_motor_set_power(STEER,0);
      ev3_motor_set_power(DRIVE_L,0);
      ev3_motor_set_power(DRIVE_R,0);
      break;

    case 14://ライン復帰
      ev3_motor_rotate(STEER,940,50,true);
      tactics->current_step = 15;
      ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
      timeCounter = 1;
      break;
    case 15:
      if(timeCounter < 37){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,30);
	ev3_motor_set_power(DRIVE_R,-30);
      }else if(timeCounter >= 37){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
	tactics->current_step = 16;
	timeCounter = 0;
      }timeCounter++;
      break;
    case 16:
      ev3_motor_rotate(STEER,-1000,50,true);
      tactics->current_step = 17;
      ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
      break;
    case 17:

      ev3_motor_set_power(STEER,0);
      if(timeCounter <= 20){
	ev3_motor_set_power(DRIVE_L,-15);
	ev3_motor_set_power(DRIVE_R,-15);
      }else if(timeCounter > 20 && (timeCounter -20) /60 % 2 == 1){
	ev3_motor_set_power(DRIVE_L,-15);
	ev3_motor_set_power(DRIVE_R,-15);
      }else if(timeCounter > 20 && (timeCounter - 20) /60 % 2 == 0){
	ev3_motor_set_power(DRIVE_L,15);
	ev3_motor_set_power(DRIVE_R,15);
      }
      timeCounter++;
      if(ev3_color_sensor_get_color(IR) == 1){
	tactics->current_step = 30;
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
	timeCounter = 0;
      }
      break;
    case 18:
      ev3_motor_rotate(STEER,830,50,true);
      tactics->current_step = 19;
      ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
      break;
    case 19:
      if(ev3_color_sensor_get_color(IR) != 1){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,-8);
	ev3_motor_set_power(DRIVE_R,8);
      }else if(ev3_color_sensor_get_color(IR) == 1){
	ev3_motor_set_power(STEER,0);
	ev3_motor_set_power(DRIVE_L,0);
	ev3_motor_set_power(DRIVE_R,0);
	tactics->current_step = 20;
      }
      break;
    case 20:
      ev3_motor_rotate(STEER,-950,40,true);
      tactics->current_step = 21;
      ev3_lcd_draw_string("tactics->current_step Now", 0, 70);
      break;
    case 21:
      ev3_motor_set_power(STEER,-2);
      ev3_motor_set_power(DRIVE_L,-17);
      ev3_motor_set_power(DRIVE_R,-15);
      if(ev3_color_sensor_get_color(IR) == 1){
	tactics->current_step = 22;
	flag = 0;
	grey = (BLACK+WHITE)/2;
	pid *my_pid = pid_make(4.0, 0.2, 0.1, grey);
	ev3_motor_reset_counts(DRIVE_L);
	ev3_motor_reset_counts(DRIVE_R);
      }
      break;
    case 22:
      count = ev3_motor_get_counts(STEER);
      forward = 14;
      pid_input(my_pid, ev3_color_sensor_get_reflect(IR));
      turn = (signed char) pid_get_output(my_pid);

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
	tactics->current_step = 6;
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
	tactics->current_step = 18;
	timeCounter = 0;
      }			timeCounter++;
    default:

      break;
    }
    if(flag==0){
      ev3_motor_steer(DRIVE_L, DRIVE_R, -forward, -turn);
    }




}
