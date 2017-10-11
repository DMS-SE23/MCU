#include <stdio.h>        				            // for printf
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f30x_conf.h"
#include "stm32f30x_dbgmcu.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_rtc.h"


#include "cmd_handle.h"
#include "common.h"
#include "config.h"
#include "gpio_control.h"
#include "hook_tasks.h"
#include "i2c_slave.h"
#include "stm32_it.h"
