#include "stm32f30x_conf.h"
// User Define Header Files
#include "project.h"
#include "gpio_control.h"
#include <stdio.h>        // for printf
#include "eeprom_control.h"
#include "main.h"
#include "debug_port.h"

void TASK_WATCHDOG()
{
  if (VAR_WATCHDOG_STATUS == 0) return;
  //DEBUG_PRINT ("COUNTER %04X\n\r",VAR_WATCHDOG_COUNTER);

  if (VAR_WATCHDOG_COUNTER-- <= 0)
  {
    DEBUG_PRINT ("[WD:TIMEOUT]\n\r");
    INC_VPM_EVENT_LOG_CNT(8); // 觸發Watchdog Reset System
    // 做Reset動作
    // ==================
    // System Warm Reset
    NVIC_SystemReset();
    // 關閉Watchdog ~~~~~
    VAR_WATCHDOG_STATUS = 0;
  }
}