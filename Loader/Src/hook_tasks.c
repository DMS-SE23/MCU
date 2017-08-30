#include "stm32f30x_conf.h"
#include "gpio_control.h"
#include "hook_tasks.h"
#include "adv7186_control.h"

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
          __OUT_D9_GPIO_OUT_12VSB_RUN_SET_LO;
          __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
          sequence1++;
          break;
      case 1:
          if (__IN_D8_GPIO_IN_12VSB_PG_TEST_HI)
          {
              __OUT_D7_GPIO_OUT_3V_5V_EN_SET_HI;
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              var_VPM_Count_Down_by_10mS = 100;         // 璶单1000mS
              sequence1++;
              return;
          }
          break;
      case 2:
          if (var_VPM_Count_Down_by_10mS-- <= 90)       // 100ms
          {
              __OUT_B4_GPIO_OUT_5V_EN_SET_HI;
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
              sequence1++;
              return;
          }
          break;
      case 3:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              var_VPM_Count_Down_by_10mS = 1000;        // 璶单10S
              sequence1++;
              return;
          }
          break;
      case 4:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
              __OUT_D14_GPIO_OUT_PWRDN_7186_SET_HI;
              sequence1++;
              return;
          }
          break;
      case 5:
          __OUT_D15_GPIO_OUT_RESET_7186_SET_HI;
          sequence1++;
          break;
      case 6:
          ADV7186_LVDS();
          sequence1++;
          break;
      case 7:
          var_VPM_Count_Down_by_10mS = 50;                  // 璶单500mS
          sequence1++;
          break;
      case 8:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
              __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_HI;
              sequence1++;
              return;
          }
          break;
      default:
          break;
  }
}

//-----------------------------------------------------------------------------
void TASK_POWER_DOWN()
{
  static unsigned char sequence2 = 0;
  static long var_VPM_Count_Down_by_10mS;
  switch (sequence2)
  {
     case 0:
          __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
	  sequence2++;
          break;
      case 1:
          __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_LO;
          var_VPM_Count_Down_by_10mS = 600;                     // 璶单6000mS~6s
          sequence2++;
          break;
      case 2:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
              __OUT_B5_GPIO_OUT_DA9063_ONKEY_BAR_SET_HI;
              var_VPM_Count_Down_by_10mS = 3000;                // 璶单30000mS~30s
              sequence2++;
              return;
          }
          break;
      case 3:
          if (var_VPM_Count_Down_by_10mS-- <= 0 || __IN_C7_GPIO_IN_EN_LED_TEST_LO) // LCD Backlight Going Low
          {
              // Turn Off Backlight
              __OUT_B14_GPIO_OUT_BACKLIGHT_EN_SET_LO;
              var_VPM_Count_Down_by_10mS = 100;                 // 璶单1S
              sequence2++;
              return;
          }
          break;
      case 4:
          if (var_VPM_Count_Down_by_10mS-- <= 0)
          {
            NVIC_SystemReset();
            sequence2++;
            return;
          }
          break;
      default:
          sequence2 = 0;                                    // Workaround, ぃ琌sequence2 initぃ0
          break;
  }
}
//-----------------------------------------------------------------------------


// ===============================
// Hook Tasks Main Processing Area
// ===============================

// 10mS HookㄧΑ
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