#include "includes.h"

void TASK_WATCHDOG()
{
  if (VAR_WATCHDOG_STATUS == 0) return;
  //DEBUG_PRINT ("COUNTER %04X\r\n",VAR_WATCHDOG_COUNTER);

  if (VAR_WATCHDOG_COUNTER-- <= 0)
  {
    DEBUG_PRINT ("[WD:TIMEOUT]\r\n");
    INC_VPM_EVENT_LOG_CNT(8); // 觸發Watchdog Reset System
    // 做Reset動作
    // ==================
    // System Warm Reset
    NVIC_SystemReset();
    // 關閉Watchdog ~~~~~
    VAR_WATCHDOG_STATUS = 0;
  }
}