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
    char rxPacket[RLYR993_MAX_PACK_SIZE];
	char txPacket[RLYR993_MAX_PACK_SIZE];
	char rxPacketSize;
	char txPacketSize;
	union
	{
		uint8_t all;
		struct
		{
			uint8_t reserved		:7,
					rxPacket		:1;
		}flags;
	}status;

}rlyr993_buffer;


/* VARIABLES -----------------------------------------------------------------*/
rlyr993_handler hRlyr993 = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static uint16_t rlyr993_make_command(char* targetString, char* command, char* parameter);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief   Initialize LoRaWAN module driver that will send AT commands and callback a function when receiving data
 * 
 * @param   tx_function     :   UART transmission function pointer
 * 
 * @param   rx_callback     :   Higher layer reception callback pointer. 
 */
void rlyr993_init(void* tx_function, void* rx_callback)
{
    hRlyr993.commandSend = tx_function;

    hRlyr993.receiveCallback = rx_callback;

    rlyr993_set_band();

    rlyr993_set_deveui();

    rlyr993_set_appeui();

    rlyr993_set_networkId();

    rlyr993_set_appkey();

    // rlyr993_set_class();

    //rlyr993_set_mode(LORAWAN);

    rlyr993_join_request();
    
}
/**
 * @brief Set LoRaWAN device unique ID
 * 
 */
void rlyr993_set_deveui(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, DEVEUI, OTAA_DEVEUI);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN application unique ID
 * 
 */
void rlyr993_set_appeui(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, APPEUI, OTAA_APPEUI);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN network unique ID
 * 
 */
void rlyr993_set_networkId(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, NWKKEY, OTAA_APPKEY);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN application key ID
 * 
 */
void rlyr993_set_appkey(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, APPKEY, OTAA_APPKEY);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN communication EU868 band
 * 
 */
void rlyr993_set_band(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, BAND, OTAA_BAND_EU868);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set Module communication mode whether it is peer to peer or LoRaWAN
 * 
 */
void rlyr993_set_mode(char* mode)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, MODE, mode);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN class A
 * 
 */
void rlyr993_set_class(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, CLASS, CLASS_A);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief LoRaWAN join request with OTAA method 
 * 
 */
void rlyr993_join_request(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_command(module_data.txPacket, JOIN, OTAA);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief   Construct AT command using the passed parameters to be sent to the module.
 * 
 * @param   command     :   AT command string.
 * 
 * @param   parameter   :   Parameter to set.
 * 
 * @return  uint16_t    :   The size of the constructed command length.
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