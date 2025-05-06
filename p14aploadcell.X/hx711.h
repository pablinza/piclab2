/* File:   hx711.h / hx711.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: HX711 24-Bit ADC for Weigh Scales PIC Driver 
 * Designed: Load cell Weigh Amplifier with PIC16F  
 * Revision history: 25.03 */
//<24:bits of Data><25-27:End bit or PGA Control> 
//The output 24 bits of data is in 2?s complement
//format. When input differential signal goes out of
//the 24 bit range, the output data will be saturated
//at 800000h (MIN) or 7FFFFFh (MAX)
#ifndef XC_HX711_H
#define	XC_HX711_H
#include <xc.h>  
/* USER PORT DEFINITION */

#define HX_SDIpin PORTBbits.RB0 //Serial Data Input fron HX711									*
#define HX_SDItris TRISBbits.TRISB0
#define HX_CLKpin PORTBbits.RB1 //Clock Output to HX711
#define HX_CLKtris TRISBbits.TRISB1
/* END USER PORT DEFINITION */
#define HX_CHA128   25  //±20mV input PGA
#define HX_CHB32    26  //±80mV input PGA
#define HX_CHA64    27  //±40mV input PGA
void HXSetup(void);
uint8_t HXStart(void);
uint24_t HXReadvalue(uint8_t mode);
#endif