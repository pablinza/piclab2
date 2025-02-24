/* Files:	ds18b20.h / ds18b20.c
 * Author:	Pablo Zarate email:pablinza@me.com
 * Summary: DS18B20 driver for PIC16F in multidrop 1-Wire bus mode
 * Need 4.7k pull-up resistor in DQ pin, with Powered mode
 * This library uses delay calls that blocking the processor
 * Revision history: 25.02 */
#include <xc.h>
#include <pic16f887.h>
#include "ds18b20.h"

/* void DS18Setup(void)
 * Set DS18B 1-Wire data pin DQ 
 * return: none*/
void DS18Setup(void)
{
    DS18pin = 0; //Set pin to Low level
    DS18tris = 1;//Set pin DQ to tri-state, idle is hight level
}

/* uint8_t DS18Reset(void)
 * Send Reset Slot to 1-Wire Bus 480u+480u 
 * return: 1 if slave presence is detected*/
uint8_t DS18Reset(void)
{
	uint8_t res;
	DS18tris = 0;	//Put bus in low level (output)
	DS18_delay_480u();//Wait for 480uS with bus in low level
	DS18tris = 1;	//Release bus to high level (input tri-state)
	DS18_delay_60u(); //Wait for 60uS before Slave Presence signal
	if(DS18pin == 0) res = 1;//Detect Slave signal
	else res = 0; //No Slave signal
	DS18_delay_420u(); //Wait for 480uS (Presence signal + 420u)
    return res;
}

/* void DS18Writebyte(uint8_t data)
 * Send Write Slot to 1-Wire Bus 
 * data: byte data to send
 * return: none*/
void DS18Writebyte(uint8_t data)
{
    uint8_t cnt = 8;
	while(cnt --)
    {
        DS18tris = 0; //Put bus in low level (output)
        if(data & 0x01) //Slot time to write 1 
        {
            DS18_delay_05u();//wait time between 5-15us
            DS18tris = 1;    //Master release bus (input tri-state)
            DS18_delay_60u();//Required time delay
        } 
        else //Slot time to write 0
		{   
            DS18_delay_60u();//wait time between 60-120us
            DS18tris = 1;    //Master release bus (input tri-state)
            DS18_delay_05u();//Required time delay
		}
      data = data >> 1;
	}
}

/* uint8_t DS18Readbyte(void)
 * Receive Read Slot from 1-Wire Bus 
 * return: byte data received*/
uint8_t DS18Readbyte(void)
{
	uint8_t cnt = 8, data = 0;
	while(cnt--)
    {
        DS18tris = 0;	//Master Put bus in low level (output)
        DS18_delay_05u();//wait time between max 15u
        DS18tris = 1;	//Master release bus (input tri-state)
        DS18_delay_05u();//wait time between before sample input
        //DS18_delay_05u();//wait time between before sample input
      	if(DS18pin) data |= 0x80; //if level is high(1)
		if(cnt) data >>= 1;
        DS18_delay_60u();// wait for recovery time
	}
	return data;
}

/* void DS18StartDev(uint8_t *romptr)
 * Match slave device and Start Conversion  
 * romptr: 8 byte (64-bit) device rom data
 * return: none*/
void DS18StartDev(uint8_t *romptr)
{
    DS18Writebyte(CMD_ROMMATCH);
    DS18Writebyte(romptr[0]); //Get Family Code 0x28
    DS18Writebyte(romptr[1]); //48-bit LSB code
    DS18Writebyte(romptr[2]);
    DS18Writebyte(romptr[3]);
    DS18Writebyte(romptr[4]);
    DS18Writebyte(romptr[5]); //48-bit MSB code
    DS18Writebyte(romptr[6]); 
    DS18Writebyte(romptr[7]); //CRC
    DS18Writebyte(CMD_CONVERT);
}

/* uint16_t DS18GetValue(uint8_t *romptr)
 * Get temperature data from slave device  
 * romptr: 8 byte (64-bit) device rom data
 * return: 16-bit data format <Sxxx><xxxx><xxxx>.<dddd>*/
uint16_t DS18GetValue(uint8_t *romptr)
{
    uint16_t result = 0;
    DS18Writebyte(CMD_ROMMATCH);
    DS18Writebyte(romptr[0]); //Get Family Code 0x28
    DS18Writebyte(romptr[1]); //48-bit LSB code
    DS18Writebyte(romptr[2]);
    DS18Writebyte(romptr[3]);
    DS18Writebyte(romptr[4]);
    DS18Writebyte(romptr[5]); //48-bit MSB code
    DS18Writebyte(romptr[6]); 
    DS18Writebyte(romptr[7]); //CRC
    DS18Writebyte(CMD_READ);
    result = (uint16_t) DS18Readbyte(); //LSB
    result = result | (((uint16_t) DS18Readbyte()) << 8); //MSB
    return result;
}

/* void DS18SetConfig(uint8_t *romptr, uint8_t data)
 * Set value resolution in slave device configuration  
 * romptr: 8 byte (64-bit) device rom data
 * data: put definitions CONFIG_xxBIT for 9 to 12-bit 
 * return: none*/
void DS18SetConfig(uint8_t *romptr, uint8_t value)
{
    DS18Writebyte(CMD_ROMMATCH);
    DS18Writebyte(romptr[0]); //Get Family Code 0x28
    DS18Writebyte(romptr[1]); //48-bit LSB code
    DS18Writebyte(romptr[2]);
    DS18Writebyte(romptr[3]);
    DS18Writebyte(romptr[4]);
    DS18Writebyte(romptr[5]); //48-bit MSB code
    DS18Writebyte(romptr[6]); 
    DS18Writebyte(romptr[7]); //CRC
    DS18Writebyte(CMD_WRITE);
    DS18Writebyte(0xFF); //Set Register T
    DS18Writebyte(0xFF); //Set Register L
    DS18Writebyte(value); //TCON 188ms
}

/* uint8_t DS18ReadROM(uint8_t *romptr)
 * Read ROM data from unique device connected on bus  
 * romptr: 8 byte (64-bit) rom data from device
 * return: retrieve rom data*/
void DS18ReadROM(uint8_t *romptr)
{
    DS18Writebyte(CMD_ROMREAD);
    romptr[0] = DS18Readbyte(); //Get Family Code 0x28
    romptr[1] = DS18Readbyte(); //48-bit LSB code
    romptr[2] = DS18Readbyte();
    romptr[3] = DS18Readbyte();
    romptr[4] = DS18Readbyte();
    romptr[5] = DS18Readbyte(); //48-bit MSB code
    romptr[6] = DS18Readbyte(); 
    romptr[7] = DS18Readbyte(); //CRC
}       