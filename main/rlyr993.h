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
#define OK						"OK"

#define TEMPERATURE             "TEMP"
#define LOCAL_TIME              "LTIME"          

#define TXPOWER                 "TXP"
#define RECEIVE                 "+RCV"

#define MODE					"MODE"		//Set the wireless work mode.
#define MODE					"MODE"		//Set the wireless work mode.
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
#define TERMINATOR				"\r\n"

#define RLYR993_MAX_PACK_SIZE   50
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void rlyr993_set_deveui(void);

void rlyr993_init(void* tx_function, void* rx_callback);
#endif /* MAIN_TRLYR993_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/