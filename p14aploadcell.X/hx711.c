/* File:   hx711.h / hx711.c
 * Author: Pablo Zarate email:pablinza@me.com
 * Comments: HX711 24-Bit ADC for Weigh Scales PIC Driver 
 * Designed: Load cell Weight Amplifier with PIC16F  
 * Revision history: 25.03 */
#include <xc.h>
#include "hx711.h"
/* void HX711(void)
 * Setup pin ports for HX711
 * Precon: none
 * Params: none
 * Return: none */
void HXSetup(void)
{
    HX_SDItris = 1; //Data input mode
    HX_CLKtris = 0; //Clock output
    HX_CLKpin = 1; //High mode to put HX in Power-Down
}
/* uint8_t HXStart(void)
 * Wakeup HX711 from PowerDown mode
 * Return 1 if HX711 es ready for retrieve command*/
uint8_t HXStart(void)
{
    HX_CLKpin = 0; //Put clock in low to Wakeup HX711
    if(HX_SDIpin == 0) return 1;
    else return 0;
}
/* uint24_t HXReadvalue(uint8_t mode)
 * Read ADC Result from HX711 Amplifier
 * mode: Channel and Gain selection HX_CHA128 CHA64 CHB32
 * Return <24-bits Data><25-27:End bit or Control>
 * Two complement result 800000h (MIN) or 7FFFFFh (MAX) */
uint24_t HXReadvalue(uint8_t mode)
{
	uint24_t value = 0;
	uint8_t i;
	for(i = 0; i < 24; i ++)
	{
		HX_CLKpin = 1;
		value <<= 1; //typ 1uS delay
		NOP();
        HX_CLKpin = 0;
		if(HX_SDIpin)
			value++; //put one to lsb bit
	}
	HX_CLKpin = 1; //Add end pulse 25
	value ^= 0x800000; //get absolute value 
    //Negative input Vr- > Vr+ scale to 000000h-7FFFFFh
    //Positive input Vr- < Vr+ scale to 800000h-FFFFFFh
	NOP();
    HX_CLKpin = 0;
	if(mode >= HX_CHB32) //Add clock pulse 26
	{
		HX_CLKpin = 1;
		NOP(); //1uS delay
		NOP(); //1uS delay
		HX_CLKpin = 0;
	}
	if(mode >= HX_CHA64) //Add clock pulse 27
	{
		HX_CLKpin = 1;
		NOP(); //1uS delay
		NOP(); //1uS delay
		HX_CLKpin = 0;
	}
	return value;
}
