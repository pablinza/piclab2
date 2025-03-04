/*
 * File:   main.c
 * Author: pablo
 *
 * Created on February 24, 2025, 10:11 PM
 */
#pragma config FOSC=INTRC_NOCLKOUT, WDTE=OFF, BOREN=OFF, PWRTE=ON, LVP=OFF
#include <xc.h>
#include <stdio.h>
#include "tm1637.h"
#define LED1pin PORTEbits.RE2 //Led de la tarjeta
#define ESW1pin PORTBbits.RB0 //KY-040 Encoder SW pin
#define ECK1pin PORTDbits.RD0 //KY-040 Encoder CK pin
#define EDT1pin PORTDbits.RD1 //KY-040 Encoder DT pin
#define LED2pin PORTDbits.RD6 //Indicador Timer
volatile __bit tickms;
uint8_t min = 0, sec = 0;
char buffer[4] = {0,0,0,0}; //Arreglo para los digitso del display
void MCUSetup(void); //Configuracion del PIC
void taskLED1(void); //Destello del led
void taskTIM1(void); //Tarea del Temporizador
uint8_t ENCGetSense(void); //Procedimiento del encoder
void __interrupt() isr(void)
{
    uint8_t res;
    if(INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        TMR0 += 131;
        tickms = 1; //Se activa cada 1ms
    }
   
}
void main(void) 
{
    MCUSetup();
    TMSetup();
    TMSetpwm(4); //Ajusta brillo del display a 11/16
    TMShow(buffer,4);
    while(1)
    {
        if(tickms) //cada 1ms
        {
            tickms = 0;
            taskLED1();
            taskTIM1();
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
    TRISDbits.TRISD6 = 0; //Salida LED2
    PORTDbits.RD6 = 0;
    OPTION_REGbits.nRBPU = 0; //Activa las pull-ups PORTB
    /* CONFIGURACION TIMER0 1MS a Fosc=8MHz*/
    OPTION_REGbits.T0CS = 0;//Modo Termporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b011; //Prescala 1:16
    TMR0 = 131; //256-(time/((pre)*(4/Fosc))) time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    INTCONbits.GIE = 1; //Habilitador global ISR
}
void taskTIM1(void)
{
    static uint8_t cfmode = 1, sw1cnt = 0;
    static uint16_t cnt = 0;
    static uint8_t latchd;
    uint8_t res;
    if(ESW1pin == 0) //Detecccion del pulsador 
    {
        if(++sw1cnt > 199) //Mantener al menos 200ms
        {
            sw1cnt = 0;
            cnt = 0;
            if(cfmode) 
            {
                cfmode = 0;
                LED2pin = 0;
            }
            else cfmode = 1;
        }
    } else sw1cnt = 0;
    if(cfmode == 0) //Opera en Modo normal
    {
        if(cnt++ >= 999) //Espera 1seg
        {
            cnt = 0;
            if(sec) sec--; //decrementa el segundo
            else
            {
                if(min)
                {
                    min--; //decrementa el minuto
                    sec = 59;
                }
                else LED2pin = 1;
            }
            buffer[0] = (char) (min/10); //Convierte a byte BCD
            buffer[1] = (char) (min%10);
            buffer[2] = (char) (sec/10);
            buffer[3] = (char) (sec%10);
            TMSetcolon = 1; //Activa los puntos : del segundo
            TMShow(buffer,4);
        }
        if(cnt == 500) 
        {
            TMSetcolon = 0; //Desactiva los puntos : del segund
            TMShow(buffer,2);
        }
    }
    else //Opera en Modo de Ajuste
    {
        if((PORTD & 0x01) != latchd) //Enmascara los bit1-bit0
        {
            latchd = PORTD & 0x01;
            res = ENCGetSense();
            if(res) //Giro horario
            {
                if(sec >= 59) 
                {
                    if(min < 9)
                    {
                        min++;
                        sec = 0;
                    }
                }
                else sec++;
            }
            else //Giro Antihorario
            {
                if(sec) sec--;
                else
                {
                    if(min)
                    {
                        min--;
                        sec = 59;
                    }
                }
            }
            buffer[0] = (char) (min/10);
            buffer[1] = (char) (min%10);
            buffer[2] = (char) (sec/10);
            buffer[3] = (char) (sec%10);
            TMSetcolon = 1;
            TMShow(buffer,4);
        }
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
uint8_t ENCGetSense(void) //Debe ejecutarse con cada cambio en CK
{
    uint8_t res, val;
    res = ECK1pin;
    val = EDT1pin;
    res = (res ^ val);  //XOR entre CK y DT
    return (res & 0x01);//Retorna el sentido de giro
} 
