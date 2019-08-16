#ifndef __GPIO_CONTROL_H
#define __GPIO_CONTROL_H

void GPIO_OUTPUT_STATUS_INIT();
// GPIO initialization
void FUNC_GPIO_INIT();
void TASK_POWER_LED_CONTROL();
void TASK_BATTERY_LED_CONTROL();
void TASK_PANEL_STATUS_DETECTION();
void SUSPEND_WAKEUP_PIN_INIT();
void POWER_OFF_WAKE_UP_PIN_INIT();
void REINIT_AFTER_WAKEUP_RESUME_FROM_SUSPEND();
void REINIT_AFTER_WAKEUP_FROM_POWER_OFF();
void SYSCLKConfig_STOP(void);

#endif
