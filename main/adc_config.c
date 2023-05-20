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
bool do_calibration = false;
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle = NULL;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void adc_config(void)
{
    //-------------ADC1 Init---------------//
   
    adc_oneshot_unit_init_cfg_t init_config1 = 
    {
        .unit_id = ADC_UNIT_1,
        
    };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = 
    {
        .bitwidth   = ADC_BITWIDTH_DEFAULT,
        .atten      = ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN0, &config));
    //-------------ADC1 Calibration Init---------------//
    
    do_calibration = adc_calibration_init(ADC_UNIT_1, ADC_ATTEN, &adc1_cali_handle);
}
uint16_t adc_read_voltage(float voltage_divider_ration)
{
    int adc_raw = 0;

    int adc_voltege = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN0, &adc_raw));

    ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC1_CHAN0, adc_raw);

    if (do_calibration) 
    {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw, &adc_voltege));

            adc_voltege = voltage_divider_ration * adc_voltege;

        ESP_LOGI(TAG, "Vd%f ADC%d Channel[%d] Cali Voltage: %d mV",voltage_divider_ration, ADC_UNIT_1 + 1, ADC1_CHAN0, adc_voltege);
    }

    return (uint16_t)adc_voltege ;
}

static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle    = NULL;
    esp_err_t ret               = ESP_FAIL;
    bool calibrated             = false;

    if (!calibrated) 
    {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = 
        {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) 
        {
            calibrated = true;
        }
    }

    *out_handle = handle;
    if (ret == ESP_OK) 
    {
        ESP_LOGI(TAG, "Calibration Success");
    } 
    else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) 
    {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } 
    else 
    {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/