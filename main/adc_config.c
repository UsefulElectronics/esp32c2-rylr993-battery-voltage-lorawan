/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  adc_config.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Apr 19, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "adc_config.h"

/* PRIVATE STRUCTURES --------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void adc_config(void)
{
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = 
    {
        .unit_id = ADC_UNIT_1,
        
    };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = 
    {
        .bitwidth   = ADC_BITWIDTH_DEFAULT,
        .atten      = EXAMPLE_ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN0, &config));
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/