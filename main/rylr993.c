/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  rylr993.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 14, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "rylr993.h"
#include "esp_log.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* PRIVATE STRUCTURES --------------------------------------------------------*/
typedef struct 
{
    void                (*commandSend)     (void *tx_buffer);
    void                (*receiveCallback) (void* rx_packet, packet_id_e packet_id);
    module_status_e     networkStatus;
    int32_t             temperature;
    uint8_t             timer;
    int  rssi;
    int  snr;
    int  dr;
    
}rylr993_handler;
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

}rylr993_buffer;


/* VARIABLES -----------------------------------------------------------------*/
rylr993_handler hRlyr993 = {0};
char packetHolder[MAX_PPACKETS][MAX_PACKET_SIZE] = {0};
const static char *TAG = "RYLR";
QueueHandle_t rlyr993_packet_queue;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static uint16_t rylr993_make_command    (char* targetString, char* command, char* parameter);
static uint16_t rylr993_make_request    (char* targetString, char* command);
static bool     rylr993_packet_parser   (uint8_t* packet);
static char *   rylr993_raw2hex         (uint8_t* dataBuffer, uint8_t dataSize);
static uint8_t * rylr993_hex2raw        (char* dataBuffer, uint8_t dataSize);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief   Initialize LoRaWAN module driver that will send AT commands and callback a function when receiving data
 * 
 * @param   tx_function     :   UART transmission function pointer
 * 
 * @param   rx_callback     :   Higher layer reception callback pointer. 
 */
void rylr993_init(void* tx_function, void* rx_callback)
{
    rlyr993_packet_queue        = xQueueCreate(10, sizeof(rylr993_buffer));

    hRlyr993.commandSend        = tx_function;

    hRlyr993.receiveCallback    = rx_callback;

    rylr993_set_band();

    rylr993_set_deveui();

    rylr993_set_appeui();

    rylr993_set_networkId();

    rylr993_set_appkey();

    rylr993_join_request();
    
}
/**
 * @brief Set LoRaWAN device unique ID
 * 
 */
void rylr993_set_deveui(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, DEVEUI, OTAA_DEVEUI);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN application unique ID
 * 
 */
void rylr993_set_appeui(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, APPEUI, OTAA_APPEUI);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN network unique ID
 * 
 */
void rylr993_set_networkId(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, NWKKEY, OTAA_APPKEY);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN application key ID
 * 
 */
void rylr993_set_appkey(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, APPKEY, OTAA_APPKEY);

    hRlyr993.commandSend(&module_data);
}

/**
 * @brief Set LoRaWAN communication EU868 band
 * 
 */
void rylr993_set_band(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, BAND, OTAA_BAND_EU868);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set Module communication mode whether it is peer to peer or LoRaWAN
 * 
 */
void rylr993_set_mode(char* mode)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, MODE, mode);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Set LoRaWAN class A
 * 
 */
void rylr993_set_class(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, CLASS, CLASS_A);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief LoRaWAN join request with OTAA method 
 * 
 */
void rylr993_join_request(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_command(module_data.txPacket, JOIN, OTAA);

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
bool rylr993_packet_separator(uint8_t* packet, uint8_t packet_size)
{
    bool validPacket = false;

    rylr993_buffer module_data = {0};

    uint8_t packetCounter = 0;

    uint8_t charCounter = 0;

    const uint8_t dontWait = 0;

    char *pToken = strtok((char *)packet, "\r\n");

    while(pToken != NULL)
    {
        strcpy(packetHolder[packetCounter], pToken);

        ++packetCounter;

        rylr993_packet_parser((uint8_t*)pToken);
        
        pToken = strtok(NULL, "\r\n");

        // ESP_LOGI(TAG, "%s", packetHolder[packetCounter]);
    }

    if(RYLR993_JOINED == rylr993_joined_check())
    {
        if(xQueueReceive(rlyr993_packet_queue, (void * )&module_data, dontWait))
        {
            hRlyr993.commandSend(&module_data);

            hRlyr993.networkStatus = RYLR993_BUSY;
        }
    }


    return validPacket;
}
static bool rylr993_packet_parser(uint8_t* packet)
{
    bool validPacket = false;

    int unused = 0;

    const uint8_t packetBase = 0;
    
    char rxPacket[MAX_PACKET_SIZE] = {0};
    
    int rxPacketSize = 0;

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
            sscanf((char*)packet, "%d:%d:%s", &unused, &rxPacketSize, rxPacket);

            
            memcpy(rxPacket, rylr993_hex2raw(rxPacket, (uint8_t) rxPacketSize), rxPacketSize);
            //First byte is the pin number and the next one is the pin state
            hRlyr993.receiveCallback((uint8_t*) rxPacket, RYLR993_PIN_CONTROL);
        }
        if(!strncmp((char*)packet ,PARAM_REPORT ,strlen(PARAM_REPORT)))
        {
            sscanf((char*)packet, "RX_1, DR %d, RSSI %d, SNR %d", &hRlyr993.dr, &hRlyr993.rssi, &hRlyr993.snr);

            validPacket = true;

            hRlyr993.networkStatus = RYLR993_JOINED;

        }
        else if(!strncmp((char*)packet ,JOIN_REPORT ,strlen(JOIN_REPORT)))
        {
            validPacket = true;

            hRlyr993.networkStatus = RYLR993_JOINED;

        }
    }
    else if(isdigit(packet[packetBase]))
    {
        validPacket = true;

        hRlyr993.temperature = atoi((char*)packet);

        hRlyr993.receiveCallback(NULL, RYLR993_TEMPERATURE);

        ESP_LOGI(TAG, "temperature is %d", (int)hRlyr993.temperature);
    }
    return validPacket;
}
/**
 * @brief Get the scrounging temperature from the module internal sensor
 * 
 */
void rylr993_get_temperature(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_request(module_data.txPacket, TEMPERATURE);

    hRlyr993.commandSend(&module_data);
}
/**
 * @brief Get the time
 * 
 */
void rylr993_get_time(void)
{
    rylr993_buffer module_data = {0};

    module_data.txPacketSize = rylr993_make_request(module_data.txPacket, LOCAL_TIME);

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
static char * rylr993_raw2hex(uint8_t* dataBuffer, uint8_t dataSize)
{
    char *hexString = (char *)malloc((dataSize * 2 + 1) * sizeof(char));

    for (size_t i = 0; i < dataSize; i++) 
    {
        sprintf(hexString + (i * 2), "%02X", dataBuffer[i]);
    }
    return hexString;
}
/**
 * @brief 
 * 
 * @param dataBuffer 
 * @param dataSize 
 * @return uint8_t* 
 */
static uint8_t * rylr993_hex2raw(char* dataBuffer, uint8_t dataSize)
{
    uint8_t *rawData = (uint8_t *)malloc((dataSize * 2 + 1) * sizeof(uint8_t));

    uint16_t dataLength = dataSize;

    for (size_t i = 0; i < dataLength; i++) 
    {
        sscanf(dataBuffer + (i * 2), "%2hhX", &rawData[i]);
    }
    return rawData;
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
void rylr993_send_data(uint8_t lorawanPort, uint8_t ack, uint8_t* dataBuffer, uint8_t dataSize)
{
    rylr993_buffer module_data = {0};

    char* dataString = rylr993_raw2hex(dataBuffer, dataSize);

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

    xQueueSendToBack(rlyr993_packet_queue, &module_data, portMAX_DELAY);
    
    // hRlyr993.networkStatus = RYLR993_BUSY;
    // hRlyr993.commandSend(&module_data);

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
static uint16_t rylr993_make_command(char* targetString, char* command, char* parameter)
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
static uint16_t rylr993_make_request(char* targetString, char* command)
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
module_status_e rylr993_joined_check(void)
{
    return hRlyr993.networkStatus;
}
/**
 * @brief Make module available for sending data
 * 
 */
void rylr993_make_available(void)
{
    hRlyr993.networkStatus = RYLR993_JOINED;
}
/**
 * @brief   The module has temperature sensor. This function works only if rylr993_get_temperature is called periodically 
 * 
 * @return  int :   the read temperature value 
 */
int rylr993_read_temperature(void)
{
    return hRlyr993.temperature;
}
/**
 * @brief   Counter function is used to updated module network status.
 * 
 * @param   taskPeriod :    Task period that is running this function.
 */
void rylr993_status_counter(uint16_t taskPeriod)
{
    //seconds
    const uint8_t timeout = 10; 
    //convert to seconds
    uint16_t counterStep = taskPeriod / 1000; 

    rylr993_buffer module_data = {0};

    if(RYLR993_BUSY == hRlyr993.networkStatus)
    {
        hRlyr993.timer += counterStep;

        if(hRlyr993.timer > timeout)
        {
            hRlyr993.timer = 0;

            rylr993_make_available();

            if(xQueueReceive(rlyr993_packet_queue, (void * )&module_data, 0))
            {
                hRlyr993.commandSend(&module_data);

                hRlyr993.networkStatus = RYLR993_BUSY;
            }
        }
    }
    else
    {
        hRlyr993.timer = 0;
    }


}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/