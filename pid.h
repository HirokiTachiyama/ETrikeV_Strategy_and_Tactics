#ifndef __PID__
#define __PID__
typedef struct pid_structure{
  double p_gain;
  double i_gain;
  double d_gain;
  double old_diff;
  double integral;
  double target;
  double output;
  double del;
}pid;
//_____________コンストラクタ_______________________
pid *pid_make(double p_gain,double i_gain,double d_gain,double target);
//______________解放____________________________________
void pid_free(pid *target_pid);

//________________設定変更など______________________
void pid_change_del(pid *target_pid,double next_del);
void pid_init(pid *init_pid, double p_gain, double i_gain, double d_gain, double diff,double integral, double target, double output);
void __pid_init(pid *init_pid,double p_gain,double i_gain,double d_gain,double target);
void pid_input(pid *target_pid,double input);
//_______________出力_______________________________
double pid_get_output(pid* target_pid);

#endif
