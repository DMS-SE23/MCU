#ifndef __PWM_CONTROL_H
#define __PWM_CONTROL_H

void INIT_PWM_GPIO(void);
int INIT_PWM(int pwm_freq);
void SET_PWM_WIDTH(int pwm_period, int duty_cycle);

#endif