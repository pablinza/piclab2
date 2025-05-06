#pragma config FOSC=INTRC_NOCLKOUT, WDTE = OFF, BOREN = OFF, LVP = OFF
#include <xc.h>
#include "hx711.h"
#define LED1pin PORTEbits.RE2 //Pin del LED1
#define SEGPORT PORTD //Puerto asigando a los segmentos de las pantallas
#define DP1pin PORTCbits.RC0 //Pin de control Pantalla DP1
#define DP2pin PORTCbits.RC1
#define DP3pin PORTCbits.RC2
#define DP4pin PORTCbits.RC3 //Pin de control Pantalla DP4
/* Mapa de conversion a 7 Segmentos Ánodo Común dp-G-F-E-D-C-B-A */
uint8_t dmap[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x83,0xF8,0x80,0x98};
uint8_t dvalue[4] = {4,3,2,1}; //Vector para guardar los cuatro digitos 7 Segmentos
uint24_t adcres; //HX711 ADC Result
volatile __bit tickms;
void taskLED(void); //Destella LED1
void taskTDM(void); //Muestra digitos en pantalla
void taskHX7(void); //Lectura HX711 y calcula peso
void int13toDECarray(uint8_t *dst, uint16_t val);
void MCUSetup(void);
void __interrupt() isr()
{
    if(INTCONbits.T0IF) //Activada cada 1ms
     {
        INTCONbits.T0IF = 0;  //Limpia bandera
        TMR0 += 131; //Reinicia contador para 1ms
        tickms = 1; //Activa bandera
    }
}
void main(void) 
{
    MCUSetup();//Configura el PIC16F
    HXSetup(); //Configura el PGA
    while(1)
    {
        if(tickms) //Validacion en cada 1ms
        {
            tickms = 0; //Limpia bandera
            taskLED(); //Destella el LED1
            taskTDM(); //Muestra el peso en las pantallas
            taskHX7(); //Lectura de PGA y calcula peso
        }
    }
}
void MCUSetup(void) //Configuracion del PIC
{
    /* CONFIGURACION OSCILADOR 8MHz*/
	OSCCONbits.IRCF = 0b111; //Fosc = 8MHz
    while(OSCCONbits.HTS == 0) {}; //Espera cambio
    /* CONFIGURACION DE PUERTOS */
    ANSEL = 0;	//Deshabilita canales AN0-AN7
    ANSELH = 0;	//Deshabilita canales AN8-AN13
    TRISC = 0xF0;	//Salidas de control DP1-DP4
    PORTC = 0x0F;	
    TRISD = 0;	//Salidas de segmentos SEGPORT
    PORTD = 0xFF; //Apaga los segmentos anodo comun
    TRISEbits.TRISE2 = 0; //Salida LED1pin
    OPTION_REGbits.nRBPU = 0; //PORTB con resistencias pull-up
    /* CONFIGURACION TIMER0 0.001s A 8MHz */
    OPTION_REGbits.T0CS = 0;//Modo Termporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b011; //Prescala 1:16
    TMR0 = 131; //256-[(time*Fosc)/(pre*4)] time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    INTCONbits.GIE = 1;
}
void taskTDM(void) //Tarea para control Pantalla t=1ms
{
    static uint8_t state = 0;
    uint8_t res;
    res = dvalue[state]; //Recupera Digito a mostrar
    switch(state)
    {
        case 0: //Activa solo el DP1
            DP4pin = 1;
            SEGPORT = dmap[res];
            DP1pin = 0;
            state++;
            break;
        case 1: //Activa solo el DP2
            DP1pin = 1;
            SEGPORT = dmap[res];
            DP2pin = 0;
            state++;
            break;
        case 2: //Activa solo el DP3
            DP2pin = 1;
            SEGPORT = dmap[res];
            DP3pin = 0;
            state++;
            break;
        case 3: //Activa solo el DP4
            DP3pin = 1;
            SEGPORT = dmap[res];
            DP4pin = 0;
            state = 0;
            break;
    }
}
void taskHX7(void) //Lectura del PGA ejecucion a cada 1ms
{
    static uint8_t state = 0;
    static uint16_t refzero = 0;
    static uint16_t cnt = 0;
    cnt = cnt + 1;
    switch(state)
    {
        case 0: //Inicia lectura del PGA
            if(HXStart()) 
            {
                adcres = HXReadvalue(HX_CHA128);
                state = 1;
            }
            break;
        case 1: //Calculo del peso y espera
            if(cnt > 499) //500ms
            {
                cnt = 0;
                adcres >>= 10; //solo 14-bit <sign><13-bit> 0-8191
                //Se resta el error 10-bit 1024 0.05% a 5Kg = 2.5gr
                adcres = adcres & 0x001FFF; //13-bit mask with sign=0
                //sin peso el valor adcres=108
                //con peso de 400gr adcres=240 incremento +132
                if(refzero) //Si ya fue ajustado refzero > 0
                { 
                    if(adcres >= refzero) adcres = (adcres - refzero);
                    else adcres = 0;
                    adcres = adcres * 3; //400gr / +132 = 3.03
                }
                else refzero = (uint16_t) adcres; //Guarda ajuste sin peso
                int13toDECarray(dvalue, (uint16_t) adcres);
                state = 0;
            }
            break;
    }
}
//Convierte un numero decimal entero de 13-bit a un arreglo char de 4 bytes.
void int13toDECarray(uint8_t *dst, uint16_t val) //13-bit integer to decimal 4-byte array  
{
    uint8_t res;
    if(val >= 1000u) //Miles
    {
        res = (uint8_t)(val / 1000u); 
        dst[3] = res; 
        val = val % 1000u;
    } else dst[3] = 0;
    if(val >= 100u) //Centenas
    {
        res  = (uint8_t) (val / 100u);
        dst[2] = res; 
        val = val % 100u;
    } else dst[2] = 0;
    if(val >= 10) //Decimales
    {
        res = (uint8_t) (val / 10);
        dst[1] = res;
        val = val % 10u;
    } else dst[1] = 0;
    res = (uint8_t) val;
    dst[0] = res;
}

void taskLED(void) //Destello del LED, ejecucion cada 1ms
{
    static uint16_t cnt = 0;
    if(++cnt > 999) 
    {
        cnt = 0;
        LED1pin = 1;
    }
    if(cnt == 200) LED1pin = 0;
}
