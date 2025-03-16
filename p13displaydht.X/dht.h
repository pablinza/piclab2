/* File:   dht.h / dht.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Summary: DHT11 & DHT22 Humidity and Temperature Sensor Driver for PIC16F
 * Comments: Use one pin Tri-state mode with pull-up 4.7k or internal enabled
 * This library uses delay from _XTAL_FREQ that blocking the processor 
 * Revision history: 25.03 
 * Protocol communication <Host Start><Respond><40-bit of Data with CRC>
 * Host Start DHT11: <LOW 18ms><HIGH 20-40u> DHT22:<LOW 1ms><HIGH 20-40u> 
 * Device Respond: <LOW 80us><HIGH 80us>
 * 40-bit Logic 0: <LOW 50us><HIGH 26-28u>
 * 40-bit Logic 1: <LOW 50us><HIGH 70us>
 * CRC Calculation: UHUM+LHUM+UTEM+LTEM = CRC*/
#ifndef XC_DHT_H
#define	XC_DHT_H
#include <xc.h>  
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#define DHT11   18
#define DHT22   1
/* USER PORT DEFINITION */
#define DHT_PORT    PORTB   //SCLKpin
#define DHT_TRIS    TRISB 
#define DHT_DEV1 0x10 //Bit4 Pin mask of DHTport 
#define DHT_DEV2 0x20 //Bit5 Pin mask
#define DHT_DEV3 0x40 //Bit6 Pin mask
/* END USER PORT DEFINITION */

typedef struct
{
    uint8_t tem; //8-bit Temperature
    uint8_t temr; 
    uint8_t hum;
    uint8_t humr;
} DHT_t;
DHT_t DHT;
void DHTSetup(uint8_t devmask);
uint8_t DHTStart(uint8_t devmask, uint8_t type);
uint8_t DHTRead(uint8_t devmask, uint8_t type, uint8_t *dst);
void DHTUpdate(uint8_t type, uint8_t *src);
#endif	/* XC_HEADER_TEMPLATE_H */

