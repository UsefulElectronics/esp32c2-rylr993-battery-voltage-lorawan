/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  rlyr993.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 14, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "rlyr993.h"

/* PRIVATE STRUCTURES --------------------------------------------------------*/
typedef struct 
{
    void  (*commandSend)     (void *tx_buffer);
    void  (*receiveCallback) (void);
}rlyr993_handler;
typedef struct 
{
    char packet[RLYR993_MAX_PACK_SIZE];
    uint16_t size;
}rlyr993_buffer;


/* VARIABLES -----------------------------------------------------------------*/
rlyr993_handler hRlyr993 = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static uint16_t rlyr993_make_command(char* targetString, char* command, char* parameter);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/

void rlyr993_init(void* tx_function, void* rx_callback)
{
    hRlyr993.commandSend = tx_function;

    hRlyr993.receiveCallback = rx_callback;
}
void rlyr993_set_deveui(void)
{
    rlyr993_buffer module_data= {0};

    module_data.size = rlyr993_make_command(module_data.packet, DEVEUI, OTAA_DEVEUI);

    hRlyr993.commandSend(&module_data);
}
void rlyr993_set_appeui(void)
{
    rlyr993_buffer module_data= {0};

    module_data.size = rlyr993_make_command(module_data.packet, APPEUI, OTAA_APPEUI);

    hRlyr993.commandSend(&module_data);
}
void rlyr993_set_appkey(void)
{
    rlyr993_buffer module_data= {0};

    module_data.size = rlyr993_make_command(module_data.packet, APPKEY, OTAA_APPKEY);

    hRlyr993.commandSend(&module_data);
}
void rlyr993_set_band(void)
{
    rlyr993_buffer module_data= {0};

    module_data.size = rlyr993_make_command(module_data.packet, BAND, OTAA_BAND_EU868);

    hRlyr993.commandSend(&module_data);
}
void rlyr993_set_class(void)
{
    rlyr993_buffer module_data= {0};

    module_data.size = rlyr993_make_command(module_data.packet, CLASS, CLASS_A);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief 
 * 
 * @param command 
 * @param parameter 
 * @return uint16_t 
 */
static uint16_t rlyr993_make_command(char* targetString, char* command, char* parameter)
{
    uint16_t stringLength = 0;

    sprintf(targetString, "%s%s%s%s%s", 
            AT, 
            command, 
            SET_VALUE, 
            parameter, 
            TERMINATOR);

    stringLength = strlen(targetString);

    return stringLength;
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/