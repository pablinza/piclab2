/* File:   pcdlcd.h / pcdlcd.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: PCD8544 with N5110 driver for PIC16F
 * Designed for Nokia 84x48 dots matrix LCD with Software SPI  
 * Revision history: 24.07 */
#include <xc.h> 
#include "pcdlcd.h"
/* Write a byte softSPI data to PCD8544   
 * Params: (data) byte data to send 
 * Return: None */
void PCDWriteSPI(uint8_t data)
{
    uint8_t bitcount = 8;
    PCD_SCK = 0;
    PCD_SCKTris = 0; //Set the SCK pin to Low
    do  // Loop 8 times
    {
        PCD_SDO = 0;
        if(data&0x80) PCD_SDOTris = 1;  //High level SDO to bit 1 
        else PCD_SDOTris = 0;           //Low level SDO to bit 0
        data <<= 1; //shift next bit into MSB..
        PCD_SCKTris = 1; //Set SCK
        NOP();    // Produces a 50% duty cycle clock
        PCD_SCK = 0;
        PCD_SCKTris = 0; //Release SCK
        bitcount--; // Count loop iterations
    } while(bitcount);
}
/* Write command to LCD   
 * Params: (cmd) byte command 
 * Return: None
 *  PCDWriteCMD(PCD_DISPLAY);*/
void PCDWriteCMD(uint8_t cmd)
{
    PCD_DCn = 0;
    PCD_DCTris = 0; //Command Mode
    PCD_CEn = 0;
    PCD_CETris = 0; //Enable Chip CE  
    PCDWriteSPI(cmd);
    PCD_CETris = 1; //Disable Chip CE
}
/* Write an character in LCD   
 * Params: (data) valid character (ASCII)
 * Return: None
 *  PCDWriteChar('@');*/
void PCDWriteChar(char data)
{
    uint8_t i = 0;
    PCD_DCTris = 1; //Data Mode
    PCD_CEn = 0;
    PCD_CETris = 0; //Enable Chip CE  
    PCDWriteSPI(0); //Write space between chars
    while(i < 5)
    {
        PCDWriteSPI(ASCII[data-0x20][i]); 
        i++;
    }
    PCD_CETris = 1; //Disable Chip CE  
}

/* Write an Decimal number in LCD   
 * Params: (num) decimal number (16-bit)
 *        (padn) digits (1-5), zero padding
 * Return: None
 *  PCDWriteNum(12, 2);*/
void PCDWriteNum(uint16_t num, uint8_t padn) //00000-65535
{
    uint8_t digs[5] = {0,0,0,0,0};
    uint8_t i, res;
    if((num > 9999) && (padn > 4))
    {   
        digs[0] = (uint8_t) num / 10000;
        num = num % 10000;
    }
    if((num > 999) && (padn > 3))
    {   
        digs[1] = (uint8_t) num / 1000;
        num = num % 1000;
    }
    if((num > 99) && (padn > 2))
    {   
        digs[2] = (uint8_t) num / 100;
        num = num % 100;
    }
    if((num > 9) && (padn > 1))
    {   
        digs[3] = (uint8_t) num / 10;
        num = num % 10;
    }
    if(num && padn) digs[4] = (uint8_t) num;
    if(padn)
    {
        i = 5-padn;
        while(i < 5)
        {
            res = digs[i] + 0x30;
            PCDWriteChar(res);
            i++;
        }
    }
}

/* Write a null string message on LCD   
 * Params: (*str) pointer to char array
 * Return: None*/
void PCDWriteMsg(char *str)
{
    while (*str)
    {
        PCDWriteChar(*str);
        str++;
    }
}
/* Write a null string message on LCD   
 * Params: (*str) pointer to const char array
 * Return: None*/
void PCDWriteMsgROM(const char *str)
{
    while (*str)
    {
        PCDWriteChar(*str);
        str++;
    }
}
/* Setup PORTS, PCD and LCD   
 * Params: none
 * Return: None*/
void PCDSetup(void)
{
    PCD_SCKTris = 1; //Tri-state to High level on SCK
    PCD_SDOTris = 1; //Tri-state to High level on SDO
    PCD_DCTris  = 1; //Tri-state to High level on DCn
    PCD_CETris  = 1; //Tri-state to High level on CEn (Data Mode)
    PCD_RSn = 0; //Apply low level on pin output mode (set tris to 0)
    PCD_RSnTris = 0; //Set ReSet pin to low level
    NOP();
    NOP();
    PCD_RSnTris = 1; //Set ReSet pin to High level
    PCDWriteCMD(PCD_FUNCTIONSET|PCD_FSETEXTENDED); //Enable H=1 
    PCDWriteCMD(PCD_SETBIAS|4); //Optimum value = sqrt(48)-3
    PCDWriteCMD(PCD_SETVOP|50); //Set contrast to 40 0-99
    PCDWriteCMD(PCD_FUNCTIONSET); //Enable H=0
    PCDWriteCMD(PCD_DISPLAY|PCD_DISPLAYNORMAL);   
    PCDClear();
}
/* Set position of cursor on LCD   
 * Params: (col) X 0-83 position
 *         (row) Y 0-5 position
 * Return: None*/
void PCDGotoXY(uint8_t col, uint8_t row)
{
    PCD_DCn = 0;
    PCD_DCTris = 0; //Command Mode
    PCD_CEn = 0;
    PCD_CETris = 0; //Enable Chip CE  
    PCDWriteSPI(PCD_SETXADDR | col);
    PCDWriteSPI(PCD_SETYADDR | row);   
    PCD_CETris = 1; //Disable Chip CE
}
/* Clear LCD and set cursor to 0,0    
 * Params: None
 * Return: None*/
void PCDClear()
{
    uint16_t i = 0;
    PCD_DCTris = 1; //Data Mode
    PCD_CEn = 0;
    PCD_CETris = 0; //Enable Chip CE  
    while(i++ < 504) //84x48 / 8 = 504
        PCDWriteSPI(0);
    PCD_DCn = 0;
    PCD_DCTris = 0; //Command Mode
    PCDWriteSPI(PCD_SETXADDR);
    PCDWriteSPI(PCD_SETYADDR);   
    PCD_CETris = 1; //Disable Chip CE
}
