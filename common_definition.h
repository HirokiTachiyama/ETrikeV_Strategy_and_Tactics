#ifndef __COMMON_DEFINITION__
#define __COMMON_DEFINITION__

#include <stdio.h>
#include <stdlib.h>

#define BLACK  5
#define WHITE 28



//戦略を宣言する
//STRATEGY={L_ALL, R_ALL, R_U_TURN, TEST}
#define STRATEGY TEST_strategy


#define MAX_STEERING_ANGLE	630
#define DRIVING_POWER		30

#define STEER	EV3_PORT_C
#define DRIVE_L	EV3_PORT_A
#define DRIVE_R	EV3_PORT_B
#define IR	EV3_PORT_3
#define TOUCH	EV3_PORT_2


//左エッジ or 右エッジ
enum edge {right,left};
enum yes_or_no {yes, no};


#endif




