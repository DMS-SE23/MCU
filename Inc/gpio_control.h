#ifndef __GPIO_CONTROL_H
#define __GPIO_CONTROL_H

void GPIO_OUTPUT_STATUS_INIT();
// GPIO initialization
extern void FUNC_GPIO_INIT();

//GPIO Output Control
/*GPIO Group A*/
/*GPIO Group B*/
// Charger On = High
// Charger Off = Low

void TASK_CHECK_BATTERY_PRESENT();
void TASK_LED_CONTROL();

void SUSPEND_WAKEUP_PIN_INIT();
void POWER_OFF_WAKE_UP_PIN_INIT();
void REINIT_AFTER_WAKEUP_RESUME_FROM_SUSPEND();
void REINIT_AFTER_WAKEUP_FROM_POWER_OFF();
void SYSCLKConfig_STOP(void);

#endif
