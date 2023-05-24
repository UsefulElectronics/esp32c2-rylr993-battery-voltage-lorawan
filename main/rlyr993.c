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
#include "esp_log.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
/* PRIVATE STRUCTURES --------------------------------------------------------*/
typedef struct 
{
    void    (*commandSend)     (void *tx_buffer);
    void    (*receiveCallback) (void);
    bool     networkStatus;
    int32_t  temperature;
    int  rssi;
    int  snr;
    int  dr;
    
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
char packetHolder[MAX_PPACKETS][MAX_PACKET_SIZE] = {0};
const static char *TAG = "RYLR";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static uint16_t rlyr993_make_command    (char* targetString, char* command, char* parameter);
static uint16_t rlyr993_make_request    (char* targetString, char* command);
static bool     rlyr993_packet_parser   (uint8_t* packet);
static char *   rlyr993_raw2hex         (uint8_t* dataBuffer, uint8_t dataSize);
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
 * @brief   Separator the packet received from the LoRaWAN module 
 * 
 * @param   packet      :   received packet data content 
 * 
 * @param   packet_size :   received packet size in bytes
 * 
 * @return  bool        :   true if packet is valid 
 */
bool rlyr993_packet_separator(uint8_t* packet, uint8_t packet_size)
{
    bool validPacket = false;

    //  char token[MAX_PACKET_SIZE] = {0};

    uint8_t packetCounter = 0;

    uint8_t charCounter = 0;

    // ESP_LOGI(TAG, "%s", packet);

    char *pToken = strtok((char *)packet, "\r\n");


    while(pToken != NULL)
    {
        strcpy(packetHolder[packetCounter], pToken);

        ++packetCounter;

        rlyr993_packet_parser((uint8_t*)pToken);
        
        pToken = strtok(NULL, "\r\n");

        // ESP_LOGI(TAG, "%s", packetHolder[packetCounter]);
    }

    return validPacket;
}
static bool rlyr993_packet_parser(uint8_t* packet)
{
    bool validPacket = false;

    const uint8_t packetBase = 0;
    
    rlyr993_buffer module_data = {0};
    
    //Offset buffer if it starts with space character 
    if(packet[packetBase] == SPACE)
    {
        ++packet;
    }
    //Module report packet check
    if(!strncmp((char*)packet ,MSG_REPORT ,strlen(MSG_REPORT)))
    {
        
        //report header ignore 
        packet = packet + strlen(MSG_REPORT) ;

        // ESP_LOGI(TAG, "%s", packet);

        if(isdigit(packet[packetBase]))
        {
            sscanf(packet, "%d:%d:%s", NULL, &module_data.rxPacketSize, module_data.rxPacket);
        }
        if(!strncmp((char*)packet ,PARAM_REPORT ,strlen(PARAM_REPORT)))
        {
            sscanf((char*)packet, "RX_1, DR %d, RSSI %d, SNR %d", &hRlyr993.dr, &hRlyr993.rssi, &hRlyr993.snr);

            validPacket = true;
        }
        else if(!strncmp((char*)packet ,JOIN_REPORT ,strlen(JOIN_REPORT)))
        {
            validPacket = true;

            hRlyr993.networkStatus = true;

        }
    }
    else if(isdigit(packet[packetBase]))
    {
        validPacket = true;

        hRlyr993.temperature = atoi((char*)packet);

        ESP_LOGI(TAG, "temperature is %d", (int)hRlyr993.temperature);
    }
    return validPacket;
}
/**
 * @brief Get the scrounging temperature from the module internal sensor
 * 
 */
void rlyr993_get_temperature(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_request(module_data.txPacket, TEMPERATURE);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Get the time
 * 
 */
void rlyr993_get_time(void)
{
    rlyr993_buffer module_data = {0};

    module_data.txPacketSize = rlyr993_make_request(module_data.txPacket, LOCAL_TIME);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief   Converts a packet of data to a hexadecimal string representation.
 *
 *          This function takes an array of unsigned characters representing a packet of data and converts it to a hexadecimal string representation.
 *
 * @param   data    : The packet of data to be converted.
 * 
 * @param   length  : The length of the packet in bytes.

 * @return  char*   : Pointer to the formed string
 */
static char * rlyr993_raw2hex(uint8_t* dataBuffer, uint8_t dataSize)
{
    char *hexString = (char *)malloc((dataSize * 2 + 1) * sizeof(char));

    for (size_t i = 0; i < dataSize; i++) 
    {
        sprintf(hexString + (i * 2), "%02X", dataBuffer[i]);
    }
    return hexString;
}
/**
 * @brief   Create send data packet to be sent over LoRaWAN
 * 
 * @param   lorawanPort :   server port number to send the packet over 
 * 
 * @param   ack         :   Acknowledgment required enable flag 
 * 
 * @param   dataBuffer  :   Data content to be sent
 * 
 * @param   dataSize    :   Data content size
 */
void rlyr993_send_data(uint8_t lorawanPort, uint8_t ack, uint8_t* dataBuffer, uint8_t dataSize)
{
    rlyr993_buffer module_data = {0};

    char* dataString = rlyr993_raw2hex(dataBuffer, dataSize);

    char atCommandPacket[MAX_PACKET_SIZE] = {0};

    sprintf(atCommandPacket, "%s%s%s%d:%d:%s%s", 
            AT, 
            SEND, 
            SET_VALUE, 
            (int)lorawanPort, 
            (int)ack, 
            dataString, 
            TERMINATOR);


    module_data.txPacketSize = strlen(atCommandPacket);

    memcpy(module_data.txPacket, atCommandPacket, module_data.txPacketSize);

    hRlyr993.commandSend(&module_data);

    free(dataString);
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

/**
 * @brief   Construct AT command using the passed parameters to get parameter from the module.
 * 
 * @param   command     :   AT command string.
 * 
 * @return  uint16_t    :   The size of the constructed command length.
 */
static uint16_t rlyr993_make_request(char* targetString, char* command)
{
    uint16_t stringLength = 0;

    sprintf(targetString, "%s%s%s%s", 
            AT, 
            command, 
            GET_VALUE,  
            TERMINATOR);

    stringLength = strlen(targetString);

    return stringLength;
}
/**
 * @brief   Check if the module has joined lorawan network 
 * 
 * @return  true if joined
 */
bool rlyr993_joined_check(void)
{
    return hRlyr993.networkStatus;
}
/**
 * @brief   The module has temperature sensor. This function works only if rlyr993_get_temperature is called periodically 
 * 
 * @return  int :   the read temperature value 
 */
int rlyr993_read_temperature(void)
{
    return hRlyr993.temperature;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/