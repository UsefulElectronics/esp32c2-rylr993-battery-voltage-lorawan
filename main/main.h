/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 11, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"


#include "uart_config.h"
#include "adc_config.h"
#include "gpio_config.h"
#include "rylr993.h"
/* MACROS --------------------------------------------------------------------*/
#define ADC_VOLTAGE_DIV_R1          820//KOhm
#define ADC_VOLTAGE_DIV_R2          1000//KOhm
#define ADC_VOLTAGE_DIV_RATIO       (float)((ADC_VOLTAGE_DIV_R1 + ADC_VOLTAGE_DIV_R2) / (ADC_VOLTAGE_DIV_R2))
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/


#endif /* MAIN_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
