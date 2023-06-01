/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/



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
#define GPIO_CONTROL_PACKET_SIZE    30
/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void main_creatSystemTasks(void);
static bool system_change_lorawan_classA(void);
static void system_send_to_queue(void *packetPointer);
static void system_lorawan_callback(uint8_t* rx_data, uint8_t packetId);
static void adc_handling_task(void *pvParameters);
static void uart_reception_task(void *pvParameters);
static void system_task(void *pvParameters);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{
   uart_config();

   adc_config();

   rylr993_init(system_send_to_queue, system_lorawan_callback);

   gpio_pin_configure(GPIO_NUM_3, GPIO_MODE_OUTPUT);

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
         rylr993_packet_separator(uartHandler.uart_rxBuffer, uartHandler.uart_rxPacketSize);
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

      rylr993_status_counter(task_period);

      vTaskDelay(task_period/portTICK_PERIOD_MS);
   }
}

static void system_task(void *pvParameters)
{
   const uint32_t task_period = 60000 * 30; //ms = 30min

   static uint8_t networkJoined = 0;

   for(;;)
   {
      networkJoined = !system_change_lorawan_classA();

      rylr993_get_temperature();

      rylr993_get_time();

      if(networkJoined)
      {
         rylr993_send_data(1, 0, &systemParam.batteryVoltage, 2);

         rylr993_send_data(1, 0, &systemParam.temperature, 1);
      }

      vTaskDelay(task_period/portTICK_PERIOD_MS);
   }
}


static void system_send_to_queue(void *packetPointer)
{
   xQueueSendToBack(uartTx_queue, packetPointer, portMAX_DELAY);
}
static void system_lorawan_callback(uint8_t* rx_data, uint8_t packetId)
{

   uint8_t packetHandler[GPIO_CONTROL_PACKET_SIZE] = {0};

   switch (packetId)
   {
      case RYLR993_TEMPERATURE:
         systemParam.temperature = rylr993_read_temperature();
         break;
      case RYLR993_PIN_CONTROL:
         memcpy(packetHandler, rx_data, GPIO_CONTROL_PACKET_SIZE);

         gpio_pin_control(rx_data[RLYR993_GPIO_NUM_POS], 
                           rx_data[RLYR993_GPIO_STATE_POS]);
         break;
      
      default:
         break;
   }
}

static bool system_change_lorawan_classA()
{
   static bool enableFlag = 1;
   if(enableFlag)
   {
      if(rylr993_joined_check())
      {
         enableFlag = false;

         rylr993_set_class();
      }
   }

   return enableFlag;

}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/


