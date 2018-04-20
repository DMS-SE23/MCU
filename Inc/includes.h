#include <stdio.h>        				            // for printf
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f30x_conf.h"
#include "stm32f30x_dbgmcu.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_rtc.h"

#include "project.h"
#include "adc_channel.h"
#include "battery_info.h"
#include "debug_port.h"
#include "eeprom_control.h"
#include "event_control.h"
#include "gpio_control.h"
#include "hook_tasks.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "main.h"
#include "protocol.h"
#include "stm32_it.h"
#include "vpm_control.h"
#include "watchdog.h"