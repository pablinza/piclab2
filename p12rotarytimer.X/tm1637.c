/* Files:	tm1637.h / tm1637.c
 * Author:	Pablo Zarate email:pablinza@me.com
 * Summary: TM1637 Four 7-Segment CA LED driver for PIC16F-Micro
 * Clock colon is connected to SEG8 of second display digit
 * This library uses delay from _XTAL_FREQ that blocking the processor 
 * Revision history: 25.02 */
#include <xc.h>
#include "tm1637.h"

/* void TMSetup(void)
 * Configure pin directions and set CLK and DIO to idle 
 * return: none*/
void TMSetup(void)
{
    TM_datatris = 0;//Input pull-ups logic 1
    TM_clktris = 0; //Input pull-ups logic 1
    TM_datapin = 1; //Dio = High
    TM_clkpin = 1;  //Clk = High
}
/* void TMStart(void)
 * Put DIO pin to low level (MCU Start bit)  
 * return: none*/
void TMStart(void)
{
    TM_clkpin = 1;  //Clk = High
    TM_datapin = 1; //Dio = High
    TM_clkdelay();
    TM_datapin = 0; //Change H to L while Clk=1  
}

/* void TMAck(void)
 * Wait for low level in DIO pin (Chip acknowledge)  
 * return: none, Warning infinity loop presence*/
void TMAck(void)
{
   TM_clkpin = 0;
   TM_clkdelay();
   TM_clkdelay();
   TM_datatris = 1; //Dio Input mode
   while(TM_datapin);
   TM_datatris = 0; //Dio Output mode
   TM_clkpin = 1;
   TM_clkdelay();
   TM_clkpin = 0;
   
}

/* void TMStop(void)
 * Put DIO pin on High level for idle state (MCU stop bit)  
 * return: none*/
void TMStop(void)
{
    TM_clkpin = 0;
    TM_clkdelay();
    TM_datapin = 0;
    TM_clkdelay();
    TM_clkpin = 1;
    TM_clkdelay();
    TM_datapin = 1; //Change L to H while Clk=1
}

/* void TMWritebyte(uint8_t byte)
 * Write byte data or command to bus  
 * byte: Data send from MCU to chip*/
void TMWritebyte(uint8_t byte)
{
    char n = 8;
    while(n --)
    {
        TM_clkpin = 0; //low level
        if(byte & 0x01) TM_datapin = 1;
        else TM_datapin = 0;
        TM_clkdelay();
        byte = byte >> 1;
        TM_clkpin = 1;
        TM_clkdelay();
    }
}

/* void TMSetpwm(uint8_t n)
 * Set brightness of display with 8 levels
 * n: levels, 0=off and 8=Max level*/
void TMSetpwm(uint8_t n)
{
    if(n) n = n | 0x08; //Mask display bits 
    TMStart();
    TMWritebyte(TM_CONTROL_CMD | n); //Write cmd with autoincrement
    TMAck();
    TMStop();
}

/* void TMShow(char *src, uint8_t num)
 * Send byte array digits to chip Display
 * *src: byte array pointer, byte in bcd o ascii format
 *  num: number of digits on byte array 1 to 7 */
void TMShow(char *src, uint8_t num) //Byte or ASCII BCD <byte1><byte2><byte2>..<<byten>
{
    char i, data;
    TMStart();
    TMWritebyte(TM_WRITEDATA_CMD); //Write cmd with autoincrement
    TMAck();
    TMStop();
    TMStart();
    TMWritebyte(TM_ADDR0_CMD); //Set display to 0 pos
    TMAck();
    for(i = 0; i < num; i++)
    {
        data = dmap[*src & 0x0F]; //ascii to integer
        if((TMSetcolon) && (i == 1)) data |= 0x80; 
        TMWritebyte(data);
        TMAck();
        src++;
    }
    TMStop();
}

/* uint8_t TMReadbyte(void)
 * Read byte data from bus DIO
 * return: Data byte received from chip*/
uint8_t TMReadbyte(void)
{
    char n = 8, byte = 0;
    while(n --)
    {
        TM_clkpin = 0; //low level
        TM_clkdelay();
        TM_clkdelay();
        TM_clkpin = 1; //low level
        if(TM_datapin) byte |= 0x80; //if level is high(1)
		if(n) byte >>= 1;
        TM_clkdelay();
        TM_clkdelay();
    }
    return byte;
}

/* uint8_t TMScan(void)
 * Read a Scan Key from TM1637 keypad testing circuit     
 * return: byte data format: <S0><S1><S2><K1><K2><1><1><1>*/
uint8_t TMScan(void)  //this procedure was not tested yet
{
    char i, data;
    TMStart();
    TMWritebyte(TM_READKEY_CMD); //Read Key scan code
    TMAck();
    data = TMReadbyte(); //Set display to 0 pos
    TMAck();
    TMStop();
    return data;
}
