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
//_____________�R���X�g���N�^_______________________
pid *pid_make(double p_gain,double i_gain,double d_gain,double target);
//______________���____________________________________
void pid_free(pid *target_pid);

//________________�ݒ�ύX�Ȃ�______________________
void pid_change_del(pid *target_pid,double next_del);
void pid_init(pid *init_pid, double p_gain, double i_gain, double d_gain, double diff,double integral, double target, double output);
void __pid_init(pid *init_pid,double p_gain,double i_gain,double d_gain,double target);
void pid_input(pid *target_pid,double input);
//_______________�o��_______________________________
double pid_get_output(pid* target_pid);

#endif
