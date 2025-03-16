#pragma config FOSC=INTRC_NOCLKOUT, WDTE=OFF, BOREN=OFF, PWRTE=ON, LVP=OFF
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcdlcd.h"
#include "dht.h"
#define _XTAL_FREQ 8000000
#define LED1pin PORTEbits.RE2 //Led de la tarjeta
volatile __bit tickms;
uint8_t buffer[5];
void taskAPP(void);
void taskLED(void);
void MCUSetup(void);
void putch(char byte)
{
    while(PIR1bits.TXIF == 0) {};
    TXREG = byte; 
}
void printHEX(uint8_t val)
{
    uint8_t res; 
    res = val >> 4;
    if(res > 9) res += 0x37;
    else res += 0x30;
    putch(res);
    res = 0x0F & val;
    if(res > 9) res += 0x37;
    else res += 0x30;
    putch(res);
}
void __interrupt() isr(void)
{
    if(INTCONbits.T0IF)
    {
        TMR0 += 130; 
        INTCONbits.T0IF = 0; 
        tickms = 1;
    }
}

void main(void) 
{
    MCUSetup();
    DHTSetup(DHT_DEV1|DHT_DEV2|DHT_DEV3); //Definicion de dht.h
    __delay_ms(2000);
    PCDSetup();
    while(1)
    {
        if(tickms)
        {
            tickms = 0;
            taskLED();
            taskAPP();
        }
    }
}
void MCUSetup(void)
{
    OSCCONbits.IRCF = 0b111; //Oscilador Interno 8MHz
    while(OSCCONbits.HTS == 0);
    ANSEL = 0; //Desactiva pines ADC AN0 al AN7
    ANSELH = 0;//Desactiva pines ADC AN8 al AN13
    TRISEbits.TRISE2 = 0; //Salida LED1
    PORTEbits.RE2 = 0;
    OPTION_REGbits.nRBPU = 0; //Activa las pull-ups PORTB
    /* CONFIGURACION TIMER0 1MS a Fosc=8MHz*/
    OPTION_REGbits.T0CS = 0;//Modo Termporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b011; //Prescala 1:16
    TMR0 = 131; //256-(time/((pre)*(4/Fosc))) time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    /* CONFIGURACION UART A 9600BPS 8MHz*/
    TXSTAbits.BRGH = 1; //Alta del Generador
    TXSTAbits.TXEN = 1; //Activa el transmisor
    RCSTAbits.CREN = 0; //Activa el receptor
    RCSTAbits.SPEN = 1; //Habilita el modulo USART
    SPBRG = 51; //Formula [8MHz/(16 * 9600)] - 1
    INTCONbits.GIE = 1; //Habilitador global ISR
}
void taskAPP()
{
    static uint8_t state = 0;
    static uint16_t cnt = 0;
    uint8_t res;
    cnt++;
    switch(state)
    {
        case 0: //Muestra mensaje inicial en pantalla
            PCDGotoXY(0,0);
            PCDWriteMsgROM("DEV HUM% TEM\'C");
            PCDGotoXY(0,1);
            PCDWriteMsgROM("--------------");
            PCDGotoXY(0,2);
            PCDWriteMsgROM(" 1  00.0  00.0");
            PCDGotoXY(0,3);
            PCDWriteMsgROM(" 2  00.0  00.0");
            PCDGotoXY(0,4);
            PCDWriteMsgROM(" 3  00.0  00.0");
            PCDGotoXY(0,5);
            PCDWriteMsgROM("--------------");
            state = 1;
            break;
        case 1: //Lectura y muestra del sensor DEV1
            if(DHTStart(DHT_DEV1, DHT11)) 
            {
                INTCONbits.GIE = 0;
                res = DHTRead(DHT_DEV1, DHT11, buffer);
                INTCONbits.GIE = 1;
                if(res)
                {
                    DHTUpdate(DHT11, buffer);
                    PCDGotoXY(24,2);
                    PCDWriteNum(DHT.hum,2);
                    PCDGotoXY(60,2);
                    PCDWriteNum(DHT.tem,2);
                }
                state = 2; //proxima espera
            }
            break;
        case 2://Espera el segundo
            if(cnt == 1000)
            {
                cnt = 0;
                state = 3;
            }
            break;
        case 3: //Lectura y muestra del sensor DEV2
            if(DHTStart(DHT_DEV2, DHT11)) 
            {
                INTCONbits.GIE = 0;
                res = DHTRead(DHT_DEV2, DHT11, buffer);
                INTCONbits.GIE = 1;
                if(res)
                {
                    DHTUpdate(DHT11, buffer);
                    PCDGotoXY(24,3);
                    PCDWriteNum(DHT.hum,2);
                    PCDGotoXY(60,3);
                    PCDWriteNum(DHT.tem,2);
                }
                state = 4; //proxima espera
            }
            break;
        case 4://Espera
            if(cnt == 1000)
            {
                cnt = 0;
                state = 5;
            }
            break;
        case 5: //Lectura y muestra del sensor DEV1
            if(DHTStart(DHT_DEV3, DHT22)) 
            {
                INTCONbits.GIE = 0;
                res = DHTRead(DHT_DEV3, DHT22, buffer);
                INTCONbits.GIE = 1;
                if(res)
                {
                    DHTUpdate(DHT22, buffer);
                    DHT.tem &= 0x7F;
                    PCDGotoXY(24,4);
                    PCDWriteNum(DHT.hum,2);
                    PCDWriteChar('.');
                    PCDWriteNum(DHT.humr,1);
                    PCDGotoXY(60,4);
                    PCDWriteNum(DHT.tem,2);
                    PCDWriteChar('.');
                    PCDWriteNum(DHT.temr,1);
                    /*putch('1');
                    putch('>');
                    printHEX(buffer[0]);
                    printHEX(buffer[1]);
                    printHEX(buffer[2]);
                    printHEX(buffer[3]);
                    printHEX(buffer[4]);
                    putch('\r');
                    putch('\n');*/
                }
                state = 6; //proxima espera
            }
            break;
        case 6://Espera el segundo
            if(cnt == 1000)
            {
                cnt = 0;
                state = 1;
            }
            break;
        
    }
}
void taskLED(void) //Blink led task
{
    static uint16_t cnt = 0;
    if(++cnt > 999) 
    {
        cnt = 0;
        LED1pin = 1;
    }
    if(cnt == 200) LED1pin = 0;
}