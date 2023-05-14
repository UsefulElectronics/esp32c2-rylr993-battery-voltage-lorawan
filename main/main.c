/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 11, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "main.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void main_creatSystemTasks(void);
static void system_send_to_queue(void *packetPointer);
static void system_lorawan_callback(char rx_data);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{
   uart_config();

   rlyr993_init(system_send_to_queue, system_lorawan_callback);

   main_creatSystemTasks();

}

static void main_creatSystemTasks(void)
{
	xTaskCreatePinnedToCore(uart_event_task, "uart event", 10000, NULL, 4, NULL, 1);

	xTaskCreatePinnedToCore(uart_transmission_task, "USART TX handling task", 10000, NULL, 4, NULL, 1);
}

static void system_send_to_queue(void *packetPointer)
{
   xQueueSendToBack(uartTx_queue, packetPointer, portMAX_DELAY);
}
static void system_lorawan_callback(char rx_data)
{

}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/


