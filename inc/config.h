#ifndef __CONFIG_H
#define __CONFIG_H

/* Includes */
// Set baudrate 
 #define Serial_Baudrate 9600

//FreeRTOS
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//system
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

//FatFS
#include "diskio.h"
#include "ff.h"

// USB
#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"

//UART
#include "serial.h"

//Audio
#include "mp3.h"
#include "Audio.h"
#include "core_cm4.h"
#include "mp3dec.h"

// Exported macro 
#define ABS(x)         (x < 0) ? (-x) : x
#define MAX(a,b)       (a < b) ? (b) : a

/* Exported functions  */
extern xSemaphoreHandle serial_tx_wait_sem ;
extern xQueueHandle serial_rx_queue ;


#endif