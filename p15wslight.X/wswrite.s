/* Procedimiento de control GRB para LED SW2812 con PIC16F
   Creado por Pablo Zarate A. email:pablinza@me.com 
   Se requiere Fosc=20MHz y definir los pines de control
   WSPORT =  Puerto y WSPIND = Pin del puerto WSADDR=Arreglo RAM
*/
#include <xc.inc>
WSPORT equ PORTD    ;Puerto de Control LED
WSPIND equ 0	    ;Pin asignado DO 
WSADDR equ 0x20	    ;Direccion RAM GRB 24-bits	
psect   barfunc,local,class=CODE,delta=2 ;PIC10/12/16
global _wswrite	    ;extern of bar function goes in the C source file
_wswrite: //W contains byte parameter in call
    BANKSEL(WSADDR)
    movlw WSADDR    ;Inicia el array
    movwf FSR	    ;Carga registro indirecto
    //bit7 G-LED primer byte del Dato de 24-bit
    bsf WSPORT,WSPIND	; 0.2u coloca DO=HIGH 
    btfss INDF,7	;+0.2u verifica el bit 7
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=0
    nop			;+0.2u 
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=1
    nop			;+0.2u
    nop			;+0.2u
    //bit6
    bsf WSPORT,WSPIND
    btfss INDF,6
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop			
    nop
    //bit5
    bsf WSPORT,WSPIND
    btfss INDF,5
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit4
    bsf WSPORT,WSPIND
    btfss INDF,4
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit3
    bsf WSPORT,WSPIND 
    btfss INDF,3
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit2
    bsf WSPORT,WSPIND
    btfss INDF,2
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit1
    bsf WSPORT,WSPIND
    btfss INDF,1
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit0
    bsf WSPORT,WSPIND
    btfss INDF,0
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    incf FSR
    //bit7 R-LED segundo byte del Dato de 24-bit
    bsf WSPORT,WSPIND	; 0.2u coloca DO=HIGH 
    btfss INDF,7	;+0.2u verifica el bit 7
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=0
    nop			;+0.2u 
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=1
    nop			;+0.2u
    nop			;+0.2u
    //bit6
    bsf WSPORT,WSPIND
    btfss INDF,6
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop			
    nop
    //bit5
    bsf WSPORT,WSPIND
    btfss INDF,5
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit4
    bsf WSPORT,WSPIND
    btfss INDF,4
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit3
    bsf WSPORT,WSPIND 
    btfss INDF,3
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit2
    bsf WSPORT,WSPIND
    btfss INDF,2
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit1
    bsf WSPORT,WSPIND
    btfss INDF,1
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit0
    bsf WSPORT,WSPIND
    btfss INDF,0
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    incf FSR
    //bit7 B-LED tercer byte del Dato de 24-bit
    bsf WSPORT,WSPIND	; 0.2u coloca DO=HIGH 
    btfss INDF,7	;+0.2u verifica el bit 7
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=0
    nop			;+0.2u 
    bcf WSPORT,WSPIND	;+0.2u coloca DO=LOW si bit=1
    nop			;+0.2u
    nop			;+0.2u
    //bit6
    bsf WSPORT,WSPIND
    btfss INDF,6
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop			
    nop
    //bit5
    bsf WSPORT,WSPIND
    btfss INDF,5
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit4
    bsf WSPORT,WSPIND
    btfss INDF,4
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit3
    bsf WSPORT,WSPIND 
    btfss INDF,3
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit2
    bsf WSPORT,WSPIND
    btfss INDF,2
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit1
    bsf WSPORT,WSPIND
    btfss INDF,1
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    //bit0
    bsf WSPORT,WSPIND
    btfss INDF,0
    bcf WSPORT,WSPIND
    nop
    bcf WSPORT,WSPIND
    nop
    nop
    return //return W result
