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

#define RLYR993_MAX_PACK_SIZE   1024
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
 * @brief LoRaWAN join request with OTAA method 
 * 
 */
void rlyr993_join_request(void);
/**
 * @brief Set Module communication mode whether it is peer to peer or LoRaWAN
 * 
 */
void rlyr993_set_mode(char* mode);

#endif /* MAIN_TRLYR993_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/