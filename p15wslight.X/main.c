#pragma config FOSC=HS, WDTE = OFF, BOREN = OFF, LVP = OFF
#include <xc.h>
#define LED1pin PORTEbits.RE2
#define BUT1pin PORTBbits.RB0
__bit tickms;
typedef struct
{
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} WS_t; 
WS_t wsled __at(0x20); //Reserva espacio RAM
void MCUSetup(void);
void taskLED1(void);
void taskAPP1(void);
extern uint8_t wswrite(void);
void __interrupt() isr()
{
    if(INTCONbits.T0IF) //Activa cada 1ms 8MHz
     {
        INTCONbits.T0IF = 0;  //Limpia bandera
        TMR0 += 100; //Reinicia contador 
        tickms = 1; //Activa bandera 1ms
    }
}
void main(void) 
{
    MCUSetup();
    while(1)
    {
        if(tickms)
        {
            tickms = 0;
            taskLED1(); //Destello led
            taskAPP1(); //Control luz WS2812
        }
    }
}

void MCUSetup(void)
{
    // Operacion FOSC=20MHz con Oscilador HS Tcy=0.2u
    ANSEL = 0;
    ANSELH = 0;
    TRISEbits.TRISE2 = 0;
    PORTDbits.RD0 = 0;
    TRISDbits.TRISD0 = 0; //Salida de control WS2812
    OPTION_REGbits.nRBPU = 0;
    /* CONFIGURACION TIMER0 0.001s A 20MHz */
    OPTION_REGbits.T0CS = 0;//Modo Termporizador
    OPTION_REGbits.PSA = 0; //Con prescala
    OPTION_REGbits.PS = 0b100; //Prescala 1:32
    TMR0 = 100; //256-[(time*Fosc)/(pre*4)] time=0.001 seg
    INTCONbits.T0IF = 0; //Limpia bandera
    INTCONbits.T0IE = 1; //Activa interrupcion del TMR0
    INTCONbits.GIE = 1;
}
void taskLED1(void) //Blink led task
{
    static uint16_t cnt = 0;
    if(++cnt > 999) 
    {
        cnt = 0;
        LED1pin = 1;
    }
    if(cnt == 200) LED1pin = 0;
}
void taskAPP1(void) //Control de luz Modo 2
{
    static uint8_t ipos, state = 0;
    static uint16_t cnt;
    uint8_t i;
    switch(state)
    {
        case 0: //Estado inicial, apaga luces
            wsled.red = 0; //Apagado luz roja
            wsled.blue = 0;//Apagado luz azul
            wsled.green = 0;//Apagado luz verde
            INTCONbits.GIE = 0;
            for(i=0;i<8;i++) //Apaga luces
                wswrite();
            INTCONbits.GIE = 1;
            cnt = 0;
            ipos = 0;
            state = 1;
            break;
        case 1: //Lectura pulsador
            if(BUT1pin == 0)
            {
                if(cnt++ > 49) //Valida cada 50ms
                {
                    cnt = 0;
                    if(ipos <= 8)
                    {
                        ipos++;
                        wsled.green = 24; //Brillo luz verde
                        wsled.red = 0; //Brillo luz roja (Apagado)
                        INTCONbits.GIE = 0;
                        for(i=0; i<ipos;i++)
                        { 
                            if(i>3) //Valida despues de n posiciones 
                            {
                                wsled.red+=10; //Incremente brillo rojo
                                if(wsled.green > 5) wsled.green -=5; //Reduce brillo
                            }
                            wswrite();
                        }
                        INTCONbits.GIE = 1;
                    }
                }
            } 
            else cnt = 0;
            break;
    }
}
