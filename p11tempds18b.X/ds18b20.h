/* Files:	ds18b20.h / ds18b20.c
 * Author:	Pablo Zarate email:pablinza@me.com
 * Summary: DS18B20 driver for PIC16F in multidrop 1-Wire bus mode
 * Need 4.7k pull-up resistor in DQ pin, with Powered mode
 * This library uses delay from _XTAL_FREQ that blocking the processor 
 * Revision history: 25.02 */
#ifndef XC_DS18B20_H
#define	XC_DS18B20_H
#include <xc.h>
#define DS18pin  PORTBbits.RB0
#define DS18tris TRISBbits.TRISB0
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#define DS18_delay_05u()    __delay_us(5)
#define DS18_delay_60u()    __delay_us(60)
#define DS18_delay_420u()   __delay_us(420)
#define DS18_delay_480u()   __delay_us(480)
#define CONFIG_9BIT     0x1F //TimeConv 93.75ms
#define CONFIG_10BIT    0x3F //         187.5ms
#define CONFIG_11BIT    0x5F //         375.0ms
#define CONFIG_12BIT    0x7F //         750.0ms

/* ROM COMMANDS*/
#define CMD_ROMSEARCH	0xF0 //Recupera el codigo ROM de los sensores conectados al bus
#define CMD_ROMREAD 	0x33 //Recupera el codigo ROM cuando hay un unico sensor conectado al bus
#define CMD_ROMMATCH	0x55 //Valida el codigo ROM para acceder a los datos del sensor
#define CMD_ROMSKIP		0xCC //Permite la lectura del sensor sin validar el codigo ROM
#define CMD_ALARMSEARCH	0xEC //Recupera el codigo ROM de los sensores que tiene establecida la alarma
/* FUNCTION COMMANDS*/
#define CMD_CONVERT 0x44	//Inicia la conversion de temperatura con resultado 2-bytes
#define CMD_WRITE	0x4E	//Escribe 3-bytes de configuracion REG_TH, REG_TL y REG_CONFIG
#define CMD_READ	0xBE	//Recupera los registros 8-bytes del sensor incluido el valor CRC
#define CMD_COPY	0x48 	//Guarda la configuracion REG_TH, REG_TL, REG_CONFIG en la EEPROM 
#define CMD_RECALL	0xB8	//Recupera la configuracion REG_TH, REG_TL, REG_CONFIG desde la EEPROM
void DS18Setup(void);
uint8_t DS18Reset(void);
void DS18Writebyte(uint8_t data);
uint8_t DS18Readbyte(void);
void DS18StartDev(uint8_t *romptr);
uint16_t DS18GetValue(uint8_t *romptr);
void DS18SetConfig(uint8_t *romptr, uint8_t value);
void DS18ReadROM(uint8_t *romptr);
#endif
