/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  templete.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 27, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "gpio_config.h"

/* PRIVATE STRUCTURES --------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief   This function configures a GPIO pin to a specified mode and sets it as a push/pull output.
 *          the pin number and pin mode are provided as parameters.
 * 
 * @param   pin_number  :   The GPIO pin number to configure.
 * 
 * @param   pin_mode    :   The mode to configure the GPIO pin in.
 */
void gpio_pin_configure(uint8_t pin_number, gpio_mode_t pin_mode)
{
    gpio_reset_pin(pin_number);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(pin_number, pin_mode);
    /* Set the GPIO level according to the state (LOW)*/
    gpio_set_level(pin_number, GPIO_RESET);
}
/**
 * @brief   Change GPIO logic level
 * 
 * @param   pin_number   :  The GPIO pin number to control.
 *  
 * @param   pin_state    :  The state to the GPIO
 */
void gpio_pin_control(uint8_t pin_number, gpio_state_e pin_state)
{
    gpio_set_level(pin_number, pin_state);
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/

