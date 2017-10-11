/**
  ******************************************************************************
  * @file    IAP/src/main.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/** @addtogroup IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

uint32_t JumpAddress;
pFunction Jump_To_Application;
uint8_t Last_Enter_Bootloader_Reason;
volatile int Bootloader_Update_OK = 0;

// Event Related Variable
volatile int VAR_1mS_Event;
volatile int VAR_10mS_Event;

volatile unsigned int VAR_1mS_Counter;
volatile unsigned int VAR_10mS_Counter;

//LED counting timer variable
volatile uint16_t VAR_TIMETICK_DELAY = 0;

/*I2C Slave event status---------------------------------------------------------*/
//I2C event is set to "1" after the i2c stop event detected in i2c interrupt handler to indicate a complete i2c packet is received.
//or after a NACK event detected in i2c err interrupt handler to indicate the host has
EventStatus i2c_event= NOEVENT;
uint8_t i2c_cmd;

/* Private function prototypes -----------------------------------------------*/
static void BootloaderMode_Enter(void);
static void UserMode_Enter(void);

static void TASK_Main_Event_Generator();
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t i = 0;
	uint16_t complete_tag = 0x00;
	uint32_t checksum_tag = 0x00;
	uint32_t checksum_tag_address = 0x00;
	uint32_t usr_area_checksum = 0x00;
	uint32_t usr_ap_end_address = 0x00;
	uint32_t checksum = 0;
	int var_delay = 0;

  Get_Platform_String(0,0);

  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
	  initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  SystemCoreClockUpdate();

  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* Flash unlock */
  FLASH_Unlock();

  /* Initialize Necessary GPIO Pins */
  BUTTON_INIT();

  /*delay a while before checking the button state*/
  for (var_delay = 0; var_delay<400000; var_delay++);

  //If jump from User Program
  if (BOOT_SIG == 0xbabecafe)
  {
  	BOOT_SIG = 0;
  	Last_Enter_Bootloader_Reason = FORCED_BY_COMMAND;
  }
  else
  {
	  if (CHECK_BUTTON_STATE())
	  {

		complete_tag = *((uint16_t*)(BootloaderAddress + FLASH_SIZE - (CMP_TAG_OFFSET + COMPLETE_TAG_SIZE)));

		if (complete_tag == COMPLETED)			//user area is completed programmed
		{
			checksum_tag_address = BootloaderAddress + FLASH_SIZE - (CHKSUM_TAG_OFFSET + CHKSUM_TAG_SIZE);
			checksum_tag = *((uint32_t*)checksum_tag_address);

			if (checksum_tag == DO_NOT_CHK)		//Do not check checksum
			{
				UserMode_Enter();
			}
			else
			{
				//checking checksum correct
				usr_area_checksum = *((uint32_t*)(BootloaderAddress + FLASH_SIZE - (USR_AREA_CHKSUM_OFFSET + USR_AREA_CHKSUM_SIZE)));
				usr_ap_end_address = *((uint32_t*)(BootloaderAddress + FLASH_SIZE - (USR_AP_END_ADD_OFFSET + AP_END_ADD_SIZE)));

				for (i = 0; i < usr_ap_end_address; i++)
				{
					checksum += *(uint8_t*)(ApplicationAddress + i);
				}

				if (checksum == usr_area_checksum)
				{
					if (checksum_tag == CHK_ONCE)	//Check once
					{
						checksum_tag = DO_NOT_CHK;
						uint8_t* tmp = (uint8_t*) &checksum_tag;
						Program_Info_Data(I_CHKSUM_TAG, tmp);
					}

					UserMode_Enter();
				}
				else
				{
					Last_Enter_Bootloader_Reason = CHKSUM_ERROR;
				}
			}

		}
		else
		{
			Last_Enter_Bootloader_Reason = USER_PROGRAM_EMPTY;
		}

	  }
	  else
	  {
	  	Last_Enter_Bootloader_Reason = PIN_PUSH;
	  }
  }


  BootloaderMode_Enter();

  return 0;

}

void BootloaderMode_Enter(void)
{
  // 初始化System Tick 用之 Counter
  VAR_1mS_Counter = 0;
  VAR_10mS_Counter = 0;
  //
  VAR_1mS_Event = 0;
  VAR_10mS_Event = 0;

  // 產生1mS時基之SysTick中斷
  // 72000000 / 1000 = 72000
  if (SysTick_Config(SystemCoreClock /1000))
  {
    /* Capture error */
    while (1);
  }

  /*Initial each GPIO output status*/
  //If jump from User Application by "jump to bootloader command", do not initialize the gpio state
  if (Last_Enter_Bootloader_Reason != FORCED_BY_COMMAND)
  {
    GPIO_INIT();
  }

  //Init I2C for firmware update
  I2C_Slave_Init();

  while (1)
  {
    TASK_Main_Event_Generator();

    // 10 mS
    if (VAR_10mS_Event == 1)
    {
      VAR_10mS_Event = 0;
      if (Last_Enter_Bootloader_Reason != FORCED_BY_COMMAND)
      {
        TASK_Hook_10mS(0);
      }
      if (Bootloader_Update_OK == 1)
      {
        TASK_Hook_10mS(1);
      }
    }

    //I2C Slave Command
    if (i2c_event == EVENT_NOTYET_READ)
    {
      i2c_cmd = Read_I2C_Slave_Cmd();
      i2c_event = EVENT_READ;

      if (i2c_cmd == 0xFE)
      {
        // Bypass Checksum Checking
        In_Bootloader_Mode_Response();
      }
      else
      {
        // Checksum Checking
        if (I2C_CMD_Check(i2c_cmd))
        {
          // Checksum Pass
          I2C_CMD_Handler(i2c_cmd);
        }
        else
        {
          // Checksum Fail
          I2C_CMD_Fail(i2c_cmd);
        }
      }
    }
  }
}

void UserMode_Enter(void)
{
	/* Test if user code is programmed starting from address "ApplicationAddress" */
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) ApplicationAddress);
      Jump_To_Application();
    }
}


// The timer base is 100uS (0.1mS)
void TASK_Main_Event_Generator()
{
  // 每1mS之Event
  if (VAR_1mS_Event)
  {
    VAR_1mS_Event = 0; // Clear 1mS Event
  }

  // 每10mSEvent
  if (VAR_10mS_Counter >= 10)
  {
    VAR_10mS_Event = 1;
    VAR_10mS_Counter = 0;
  }
}



#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
