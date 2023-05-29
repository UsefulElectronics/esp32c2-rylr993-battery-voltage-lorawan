
/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  gpio_config.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 27, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "driver/gpio.h"
#include "esp_log.h"
/* MACROS --------------------------------------------------------------------*/

/* ENUMERATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef enum
{
    GPIO_RESET = 0,
    GPIO_SET
}gpio_state_e;
/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief   Change GPIO logic level
 * 
 * @param   pin_number   :  The GPIO pin number to control.
 *  
 * @param   pin_state    :  The state to the GPIO
 */
void gpio_pin_control(uint8_t pin_number, gpio_state_e pin_state);
/**
 * @brief   This function configures a GPIO pin to a specified mode and sets it as a push/pull output.
 *          the pin number and pin mode are provided as parameters.
 * 
 * @param   pin_number  :   The GPIO pin number to configure.
 * 
 * @param   pin_mode    :   The mode to configure the GPIO pin in.
 */
void gpio_pin_configure(uint8_t pin_number, gpio_mode_t pin_mode);


#endif /* GPIO_CONFIG_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/