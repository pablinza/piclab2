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
#include <xc.h>
#include "dht.h"
/* void DHTSetup(void)
 * Pin configuration for Sensor Driver
 * External pull-ups or internal will be required   
 * devmask: Pin mask OR definitions, example (DEV1|DEV2..DEV7)*/
void DHTSetup(uint8_t devmask)
{ 
    DHT_PORT &= ~devmask; //Set port pins in low level
    DHT_TRIS |= devmask; //Set port pins in tri-state input
}

uint8_t DHTStart(uint8_t devmask, uint8_t type)
{
    static uint8_t state = 0;
    if(state == 0)
    {
        DHT_PORT &= ~devmask; 
        DHT_TRIS &= ~devmask; //Set LOW to Request 
        state = type;
    }
    else 
    {
        state = state - 1;
        if(state == 0) 
        {
            DHT_TRIS |= devmask; //Release to HIGH 
            return 1;
        }
    }
    return 0;
}

/* uint8_t DHTRead(uint8_t devmask, uint8_t type, uint8_t *dst)
 * Two-Step task to Read a raw 5-byte packet from DHT Sensor
 * devmask: sensor pin mask definitions to read
 * type: input to indicate device mode DHT11 or DHT22
 * *dst: buffer with 5-byte of data received from sensor 
 * Return: 1 if process complete with CRC correct */
uint8_t DHTRead(uint8_t devmask, uint8_t type, uint8_t *dst)
{
    uint8_t bitcnt = 0, byte = 0, pos = 0, cnt, crc;  
    cnt = 0;
    while(DHT_PORT & devmask) //Wait HIGH for 20-40us
    {   
        //__delay_us(1);
        if(++cnt > 40) return 0;
    } 
    cnt = 0;
    while((DHT_PORT & devmask) == 0) //Wait LOW for 80us Response
    {   
        //__delay_us(1);
        if(++cnt > 79) return 0;
    } 
    cnt = 0;
    while(DHT_PORT & devmask) //Wait HIGH for 80us
    {   
        //__delay_us(1);
        if(++cnt > 79) return 0;
    } 
    //5-byte read data from sensor 50u+30-70u * 40 = 3.2 - 4.8 ms
    while(1)
    {
        cnt = 0;
        while((DHT_PORT & devmask) == 0) //Wait LOW for 50us
        {   //Wait LOW for 50us
           //__delay_us(1);
           if(++cnt > 54) return 0;
        } 
        __delay_us(30); //delay for bit logic 26-28u= bit0 or 70u= bit1
        if(DHT_PORT & devmask) byte |= 0x01;
        if(++bitcnt > 7)
        {
            bitcnt = 0;
            dst[pos] = byte;
            if(++pos > 4)
            {   //Check if CRC is OK
                crc = dst[0];
                crc += dst[1];
                crc += dst[2];
                crc += dst[3];
                if(crc == dst[4]) 
                    return 1;
                else return 0;
            }                 
        } else byte <<= 1;
        cnt = 0;
        while(DHT_PORT & devmask) 
        {   ////Wait HIGH for end 35u or 70u
            //__delay_us(1);
            if(++cnt > 69) return 0;
        } 
    }
    return 0; 
}
/* void DHTUpdate(uint8_t type, uint8_t *src)
 * Update DHT structure (Temp & Hum) from raw 5-byte packet
 * type: input to indicate device mode DHT11 or DHT22
 * *src: buffer with 5-byte of raw data from sensor 
 * Return: RHT Structure updated. In DHT22 sensor check MSB bit 
 *         of decimal temperature was set with negative result */
void DHTUpdate(uint8_t type, uint8_t *src)
{
    uint16_t value;
    if(type == DHT11) //Only Interger values
    {
        DHT.hum = (uint8_t) src[0];
        DHT.humr = 0;
        DHT.tem = (uint8_t) src[2];
        DHT.temr = 0;
    }
    else //DHT22 type model 
    {
        value = src[0];
        value = (value << 8) + src[1];
        DHT.hum = (uint8_t)(value / 10);
        DHT.humr = (uint8_t) (value % 10);
        value = src[2] & 0x7F; //Mask Sign bit
        value = (value << 8) + src[3];
        DHT.tem = (uint8_t) (value / 10);
        DHT.temr = (uint8_t) (value % 10);
        //Set MSB bit only with negative temperature
        if(src[2] & 0x80) DHT.tem |= 0x80; 
    }
}
