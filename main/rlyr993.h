/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  rlyr993.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  May 13, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_RLYR993_H_
#define MAIN_RLYR993_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
/* MACROS --------------------------------------------------------------------*/
#define AT						"AT+"

#define RESETART				"ATZ"		//Software RESET
#define APPKEY                  "APPKEY"
#define NWKKEY                  "NWKKEY"
#define APPEUI                  "APPEUI"
#define DEVEUI                  "DEUI"

#define OTAA_BAND_EU868         "5"
#define OTAA_DEVEUI             "01:02:03:04:05:06:07:77"
								
#define OTAA_APPEUI             "0E:0D:0D:01:0E:01:02:0F"
								
#define OTAA_APPKEY             "BB:AD:D2:16:D5:E2:61:58:38:57:7E:A2:8C:09:30:D4"
								 
#define BAND					"BAND"		//Set RF Frequency
#define CLASS                   "CLASS"

#define CLASS_A                 "A"  
#define CLASS_B                 "B"     
#define CLASS_C                 "C"   

#define JOIN                    "JOIN"
#define OTAA					"1"

#define MODE					"OPMODE"
#define LORAWAN					"0"
#define PEER2PEER				"1"
#define OK						"OK"

#define TEMPERATURE             "TEMP"
#define LOCAL_TIME              "LTIME"          

#define TXPOWER                 "TXP"
#define RECEIVE                 "+RCV"

#define IPR						"IPR"		//Set the UART baud rate
#define PARAMETER				"PARAMETER"//Set the RF parameters

#define ADDRESS					"ADDRESS"	//Set the ADDRESS ID of module LoRa.
#define NETWORKID				"NETWORKID"	//Set the network ID.
#define CRFOP					"CRFOP"		//Set the RF output power.
#define SEND					"SEND"		//Send data to the appointed address by Command Mode.
#define RCV						"RCV"		//Show the received data actively
#define UID						"UID"		//To inquire module ID. 12BYTES
#define VER						"VER"

#define READY					"READY"

#define GET_VALUE				"=?"
#define SET_VALUE				"="
#define TERMINATOR				"\r"
#define RESPONSE_ENDING			"\r\n"
#define SPACE					0x20


#define MSG_REPORT				"+EVT:"
#define PARAM_REPORT			"RX_1"
#define JOIN_REPORT				"JOINED"

#define RLYR993_MAX_PACK_SIZE   1024

#define MAX_PPACKETS			5
#define MAX_PACKET_SIZE			100
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief Set LoRaWAN device unique ID
 * 
 */
void rlyr993_set_deveui(void);
/**
 * @brief   Initialize LoRaWAN module driver that will send AT commands and callback a function when receiving data
 * 
 * @param   tx_function     :   UART transmission function pointer
 * 
 * @param   rx_callback     :   Higher layer reception callback pointer. 
 */
void rlyr993_init(void* tx_function, void* rx_callback);
/**
 * @brief Set LoRaWAN class A
 * 
 */
void rlyr993_set_class(void);
/**
 * @brief Set LoRaWAN communication EU868 band
 * 
 */
void rlyr993_set_band(void);
/**
 * @brief Set LoRaWAN application key ID
 * 
 */
void rlyr993_set_appkey(void);
/**
 * @brief Set LoRaWAN application unique ID
 * 
 */
void rlyr993_set_appeui(void);
/**
 * @brief Set LoRaWAN network unique ID
 * 
 */
void rlyr993_set_networkId(void);
/**
 * @brief Get the scrounging temperature from the module internal sensor
 * 
 */
void rlyr993_get_temperature(void);
/**
 * @brief Get time
 * 
 */
void rlyr993_get_time(void);
/**
 * @brief   Create send data packet to be sent over LoRaWAN
 * 
 * @param   lorawanPort :   server port number to send the packet over 
 * 
 * @param   ack         :   Acknowledgment required enable flag 
 * 
 * @param   dataBuffer  :   Data content to be sent
 */
void rlyr993_send_data(uint8_t lorawanPort, uint8_t ack, uint8_t* dataBuffer, uint8_t dataSize);
/**
 * @brief LoRaWAN join request with OTAA method 
 * 
 */
void rlyr993_join_request(void);
/**
 * @brief 	Separates the packet received from the LoRaWAN module 
 * 
 * @param   packet      :   received packet data content 
 * 
 * @param   packet_size :   received packet size in bytes
 * 
 * @return  bool        :   true if packet is valid 
 */
bool rlyr993_packet_separator(uint8_t* packet, uint8_t packet_size);
/**
 * @brief Set Module communication mode whether it is peer to peer or LoRaWAN
 * 
 */
void rlyr993_set_mode(char* mode);
/**
 * @brief   Check if the module has joined lorawan network 
 * 
 * @return  true if joined
 */
bool rlyr993_joined_check(void);
/**
 * @brief   The module has temperature sensor. This function works only if rlyr993_get_temperature is called periodically 
 * 
 * @return  int :   the read temperature value 
 */
int rlyr993_read_temperature(void);
#endif /* MAIN_TRLYR993_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/