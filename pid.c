#include "pid.h"
#include "common_definition.h"

//構造体"pid"の初期化
pid *pid_make(double p_gain,double i_gain,double d_gain,double target){
  pid *new_pid = (pid *) malloc(sizeof(pid));
  __pid_init(new_pid,p_gain,i_gain,d_gain,target);
  return new_pid;
}

//pid_initのラッパー関数
void __pid_init(pid *init_pid,double p_gain,double i_gain,double d_gain,double target){
  pid_init(init_pid,p_gain,i_gain,d_gain,0,0,target,0);
}

 void pid_init(pid *init_pid, double p_gain, double i_gain, double d_gain, double diff,double integral, double target, double output){
  init_pid->p_gain = p_gain;
  init_pid->i_gain = i_gain;
  init_pid->d_gain = d_gain;
  init_pid->old_diff = diff;
  init_pid->integral = integral;
  init_pid->target = target;
  init_pid->output = output;
  init_pid->del = DEL;
 }

void pid_change_del(pid *target_pid,double next_del){
  target_pid->del = next_del;
}
void pid_free(pid *target_pid){
  free(target_pid);
}

void pid_input(pid *target_pid,double input){
  double p,i,d;
  double diff = input - target_pid->target;
  target_pid->integral += (target_pid->old_diff + diff)/2 * target_pid->del;
  p =  target_pid->p_gain * diff;
  i = target_pid->i_gain * target_pid->integral;
  d = target_pid->d_gain * (diff - target_pid->old_diff)/target_pid->del;
  target_pid->old_diff = diff;
  target_pid->output = p + i + d;
}

double pid_get_output(pid* target_pid){
  return target_pid->output;
}

