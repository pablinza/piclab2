/* Files:	tm1637.h / tm1637.c
 * Author:	Pablo Zarate email:pablinza@me.com
 * Summary: TM1637 Four 7-Segment CA LED driver for PIC16F-Micro
 * Clock colon is connected to SEG8 of second display digit
 * This library uses delay from _XTAL_FREQ that blocking the processor 
 * Revision history: 25.02
 * CLK Clock input: Fmax=500kHz(250k for read), 0.1(setup) + 0.4(width) + 0.1(hold) + 1u(wait)
 * DIO Data in/out: Input data changes at the low level of SLCK and is transmitted at the high level of SCLK.
 * Every byte transmitted, an ACK will be generate by chip on the falling edge of eighth clock.
 * Protocol format: <Start><CMD><Ack><Writebyte1><Ack>..<WritebyteN><Ack><Stop>
 *    for Read Key: <Start><CMD><Ack><Readbyte1><Ack><Stop> */
#ifndef XC_TM1637_H
#define	XC_TM1637_H
#include <xc.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#define TM_clkpin   PORTBbits.RB2   //SCLKpin
#define TM_clktris  TRISBbits.TRISB2 						
#define TM_datapin  PORTBbits.RB1   //SDATpin 						
#define TM_datatris TRISBbits.TRISB1
#define TM_clkdelay() __delay_us(2)
#define TM_CONTROL_CMD 0x80 //Command to Display control(last bits 000=Off, 111=Max)
#define TM_WRITEDATA_CMD 0x40 //Command to Write data on display 
#define TM_READKEY_CMD   0x42 //Command to Read key scan data
#define TM_ADDR0_CMD 0xC0 //Command to set address to pos 00h
char TMSetcolon = 0; //1 = display clock ':' on TM1637 4 Clock Led Module
const char dmap[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
void TMSetup(void);
void TMStart(void);
void TMAck(void);
void TMStop(void);
void TMWritebyte(uint8_t byte);
void TMSetpwm(uint8_t n); //from 0=off to 8=max
void TMShow(char *src, uint8_t num);
uint8_t TMReadbyte(void);
uint8_t TMScankey(void); //this procedure was not tested yet
#endif
