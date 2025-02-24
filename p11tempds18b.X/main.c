/*
 * File:   main.c
 * Author: pablo
 *
 * Created on February 21, 2025, 5:22 PM
 */
#pragma config FOSC=INTRC_NOCLKOUT, WDTE = OFF, BOREN = OFF, LVP = OFF
#include <xc.h>
#include <stdio.h>
#include "ds18b20.h"
#define LED1pin PORTEbits.RE2 //Led de la tarjeta
#define BUT1pin PORTBbits.RB1 //Push button to read ROM Devices
uint8_t dev0rom[8]={0x28,0xFF,0x97,0xD8,0x61,0x16,0x04,0x9A};  //ROM code 1 in main program
uint8_t dev1rom[8]={0x28,0xFF,0xA0,0xD5,0x61,0x16,0x04,0xD2};
uint8_t dev2rom[8]={0x28,0xFF,0x5C,0x8A,0x61,0x16,0x03,0x94};
uint8_t tem, temr;
volatile __bit tickms;
void setupMCU(void);
void taskDS18(void); //Lectura de sensores
void taskBUT1(void); //Pulsador para leer rom
void taskLED1(void); //Destello del led
void putch(char byte);
void printHEX(uint8_t val);
void printByte(uint8_t val);
void __interrupt() isr(void)
{
    uint8_t res;
    if(INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        TMR0 += 131;
        tickms = 1;
    }
   
}
void main(void) 
{
    setupMCU();
    DS18Setup();
    INTCONbits.GIE = 0;
    DS18Reset();
    DS18SetConfig(dev0rom, CONFIG_10BIT); //TConv 187ms
    DS18Reset();
    DS18SetConfig(dev1rom, CONFIG_10BIT); //TConv 187ms
    DS18Reset();
    DS18SetConfig(dev2rom, CONFIG_10BIT); //TConv 187ms
    INTCONbits.GIE = 1;
    while(1)
    {
        if(tickms) //cada 1ms
        {
            tickms = 0;
            taskDS18();
            taskBUT1();
            taskLED1();
        }
    }
}
void setupMCU(void)
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
    INTCONbits.PEIE = 0; //Interrupcion de perifericos 
    INTCONbits.GIE = 1; //Habilitador global ISR
}
void taskDS18(void)
{
    static uint8_t state = 0, devi = 0;
    static uint16_t cnt = 0;
    uint16_t res;
    switch(state)
    {
        case 0: //Verifica y inicia conversion del sensor
            INTCONbits.GIE = 0;
            if(DS18Reset()) //Si hay presencia de sensor
            {
                if(devi == 0) DS18StartDev(dev0rom);
                if(devi == 1) DS18StartDev(dev1rom);
                if(devi == 2) DS18StartDev(dev2rom);
                cnt = 200; //TConv 10-bit min 187ms
                state = 1; 
            }
            else 
            {
                cnt = 1000; //Espera proxima lectura 1s
                state = 2;
            }
            INTCONbits.GIE = 1;
            break;
        case 1://Lectura del sensor por turno
            if(cnt-- == 0) //TConv 10-bit 187
            {
                INTCONbits.GIE = 0;
                if(DS18Reset())
                {
                    if(devi == 0) res=DS18GetValue(dev0rom);
                    if(devi == 1) res=DS18GetValue(dev1rom);
                    if(devi == 2) res=DS18GetValue(dev2rom);
                }
                INTCONbits.GIE = 1;
                temr = (uint8_t) (res & 0x000F);
                temr = (100/16) * temr; //1/16 = 0.0625
                tem = (uint8_t) ((res & 0x7FFF) >> 4); 
                
                /*Muestra informacion de temperatura*/
                putch('T');
                printByte(devi);
                putch(':');
                printByte(tem);
                putch('.');
                if(temr < 10) putch('0'); //Padding
                printByte(temr);
                putch('\n');
                if(devi++ >= 2) devi = 0;
                
                cnt = 800; //Espera 800ms 
                state = 2; //Next state to wait
            }
            break;
        case 2: //Intervalo de espera entre intentos
            if(cnt-- == 0) state = 0;
            break;
    }
}
void taskBUT1(void)
{
    static uint8_t state = 0, cnt;
    switch(state)
    {
        case 0: //Detecta nivel bajo en pulsador
            if(BUT1pin == 0)
            {
                if(cnt++ > 200) //Pulsador presionado min 200ms
                    state = 1;
            } else cnt = 0;
            break;
        case 1: //Realiza del codigo ROM sensor
            INTCONbits.GIE=0;
            if(DS18Reset())
            {
                DS18ReadROM(dev0rom);
                printf("ROM CODE:");
                printHEX(dev0rom[0]);
                printHEX(dev0rom[1]);
                printHEX(dev0rom[2]);
                printHEX(dev0rom[3]);
                printHEX(dev0rom[4]);
                printHEX(dev0rom[5]);
                printHEX(dev0rom[6]);
                printHEX(dev0rom[7]);
                printf("\n");
            }
            INTCONbits.GIE=1;
            cnt = 0;
            state++;
            break;
        case 2: //Espera liberacion del pulsador
            if(BUT1pin)
            {
                if(cnt++ > 200) //Pulsador liberado min 200ms
                    state = 0;
            } else cnt = 0;
    }
}

void taskLED1(void) //Destello de LED1 1Hz al 20%
{
    static uint16_t cnt = 0;
    if(cnt++ > 999) 
    {
        cnt = 0;
        LED1pin = 1; //Activa LED
    }
    if(cnt == 200) LED1pin = 0; //Apaga LED
}
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
void printByte(uint8_t val)
{
    uint8_t res, pad = 0;
    if(val >= 100)
    {
        res  = (val / 100u);
        putch(res + 0x30); 
        val = val % 100u;
        pad = 1;
    } else if(pad) putch(0x30);
    if(val > 10)
    {
        res = val / 10u;
        putch(res + 0x30); 
        val = val % 10u;
        pad = 1;
    } else if(pad) putch(0x30);
    res = val;
    putch(res + 0x30);
}

