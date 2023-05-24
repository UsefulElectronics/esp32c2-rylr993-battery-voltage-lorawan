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

/* PRIVATE STRUCTURES ---------------------------------------------------------*/
typedef struct 
{
   uint16_t batteryVoltage;
   uint8_t  temperature;

}system_param_t;
/* VARIABLES -----------------------------------------------------------------*/
const static char *TAG = "MAIN";
system_param_t systemParam = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void main_creatSystemTasks(void);
static bool system_change_lorawan_classA(void);
static void system_send_to_queue(void *packetPointer);
static void system_lorawan_callback(char rx_data);
static void adc_handling_task(void *pvParameters);
static void uart_reception_task(void *pvParameters);
static void system_task(void *pvParameters);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{
   uart_config();

   adc_config();

   rlyr993_init(system_send_to_queue, system_lorawan_callback);

   main_creatSystemTasks();

}

static void main_creatSystemTasks(void)
{
	xTaskCreatePinnedToCore(uart_event_task, "uart event", 10000, NULL, 4, NULL, 0);

	xTaskCreatePinnedToCore(uart_transmission_task, "USART TX handling task", 10000, NULL, 4, NULL, 0);

   xTaskCreatePinnedToCore(uart_reception_task, "USART RX handling task", 10000, NULL, 4, NULL, 0);

   xTaskCreatePinnedToCore(adc_handling_task, "ADC handling task", 10000, NULL, 4, NULL, 0);

   xTaskCreatePinnedToCore(system_task, "system task", 10000, NULL, 4, NULL, 0);

}

static void uart_reception_task(void *pvParameters)
{
   uartHandler_t uartHandler = {0};
   for(;;)
   {
      //Waiting for UART packet to get received.
      if(xQueueReceive(uartRxStore_queue, (void * )&uartHandler, portMAX_DELAY))
      {
         rlyr993_packet_separator(uartHandler.uart_rxBuffer, uartHandler.uart_rxPacketSize);
      }
   }
}
static void adc_handling_task(void *pvParameters)
{
   const uint16_t task_period = 1000; //ms

   const float voltage_divider = (ADC_VOLTAGE_DIV_R1 + ADC_VOLTAGE_DIV_R2) / (ADC_VOLTAGE_DIV_R2 + 0.0);
   for(;;)
   {
      systemParam.batteryVoltage = adc_read_voltage(voltage_divider);

      vTaskDelay(task_period/portTICK_PERIOD_MS);
   }
}

static void system_task(void *pvParameters)
{
   const uint16_t task_period = 30000; //ms

   static uint8_t networkJoined = 0;

   uint8_t dataBuffer[2] = {0x13, 0x22};
   for(;;)
   {
      networkJoined = !system_change_lorawan_classA();

      rlyr993_get_temperature();

      rlyr993_get_time();

      if(networkJoined)
      {
         rlyr993_send_data(1, 0, systemParam.batteryVoltage, 2);

         rlyr993_send_data(1, 0, systemParam.temperature, 1);
      }

      vTaskDelay(task_period/portTICK_PERIOD_MS);
   }
}


static void system_send_to_queue(void *packetPointer)
{
   xQueueSendToBack(uartTx_queue, packetPointer, portMAX_DELAY);
}
static void system_lorawan_callback(char rx_data)
{

}

static bool system_change_lorawan_classA()
{
   static bool enableFlag = 1;
   if(enableFlag)
   {
      if(rlyr993_joined_check())
      {
         enableFlag = false;

         rlyr993_set_class();
      }
   }

   return enableFlag;

}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/


