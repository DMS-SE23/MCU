#include "includes.h"

//===============================
// Hook Task List Area
//===============================
void TASK_POWER_UP()
{
  static unsigned char sequence1 = 0;
  static long var_VPM_Count_Down_by_10mS;

  switch (sequence1)
  {
      case 0:
          __OUT_A1_GPIO_OUT_POWER_ON_EN_BAR_SET_HI;
          __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
          sequence1++;
          break;
      case 1:
          __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_LO;
          var_VPM_Count_Down_by_10mS = 80;
          sequence1++;
          break;
      case 2:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
              __OUT_B5_GPIO_OUT_CPU_ONKEY_BAR_SET_HI;
              sequence1++;
              return;
          }
          break;
      case 3:
          __OUT_D15_GPIO_OUT_PMIC_ON_REQ_SET_HI;
          sequence1++;
          break;
      case 4:
          if (__IN_B2_GPIO_IN_PG_VCC_3V3_TEST_HI)
          {
              sequence1++;
              return;
          }
          break;
      case 5:
          __OUT_A7_GPIO_OUT_UP_CPU_RST_BAR_SET_HI;
          sequence1++;
          break;
      default:
          break;
  }
}

//-----------------------------------------------------------------------------
void TASK_POWER_DOWN()
{
  static unsigned char sequence2 = 0;
  
  switch (sequence2)
  {
     case 0:
          GPIO_OUTPUT_STATUS_INIT();
	sequence2++;
          break;
      case 1:
          NVIC_SystemReset();
          sequence2++;
          break;
      default:
          sequence2 = 0;                                    // Workaround, 不知是何原因sequence2 init不為0
          break;
  }
}
//-----------------------------------------------------------------------------


// ===============================
// Hook Tasks Main Processing Area
// ===============================

// 10mS Hook主函式
void TASK_Hook_10mS(int power_type)
{
    if (power_type == 0)
    {
	    TASK_POWER_UP();
    }
    if (power_type == 1)
    {
	    TASK_POWER_DOWN();
    }
}
//=============================================================================