#include "config.h"

xSemaphoreHandle serial_tx_wait_sem = NULL;
xQueueHandle serial_rx_queue = NULL;

void vApplicationTickHook()
{
}

static void MP3_play_task(void *pvParameters)
{
	mp3_process();
	while(1);
}

static void serial_task(void *pvParameters)
{
	while(1);
}

int main(void)
{

	/* Create UART */
	vSemaphoreCreateBinary(serial_tx_wait_sem);
	serial_rx_queue = xQueueCreate(10, sizeof(serial_msg));

	/* Create a task to play music from micro USB. */
  	xTaskCreate(MP3_play_task,
             (signed portCHAR *) "Play music from micro USB",
             1024 /* stack size */, NULL,
             tskIDLE_PRIORITY + 5, NULL);

  	/* Create a task to execute serial communication. */
  	xTaskCreate(serial_task,
             (signed portCHAR *) "serial communication",
             1024 /* stack size */, NULL,
             tskIDLE_PRIORITY + 3, NULL);

 	 vTaskStartScheduler();
  	return 0;
}