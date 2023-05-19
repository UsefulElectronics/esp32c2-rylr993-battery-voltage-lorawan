/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  adc_config.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 19, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

const static char *TAG = "ADC";
/* MACROS --------------------------------------------------------------------*/
#define ADC1_CHAN0                  ADC_CHANNEL_0
#define EXAMPLE_ADC_ATTEN           ADC_ATTEN_DB_11
/* ENUMERATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_ADC_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/