# PICMCU LAB2 pablinza@me.com
Programacion del Microcontrolador PIC16F887 LAB2

<p align="center">
  <img src="/images/pic1.png"></img>
</p>
Carpeta con los proyectos generados por MPLABX utilizando el compilador XC8 para el modelo:
- PIC16F887 a 8MHz, utilamos la placa B8P40 descrita en el siguinte enlace [Click Aqui](https://github.com/pablinza/elt436)
Los archivos con extension .c corresponden al codigo fuente de un programa o practica
Los archivos con extension .h y .c con mismo nombre, son procedimientos y funciones basicas
Los archivos con extension .hex representan el codigo de maquina generado para el PIC
Los archivos con extension .png corresponden a una imagen de esquema de circuito

Los proyectos se crearon con la version MPLABX 6.20 y el compilador XC8 3.0 version FREE
Practicas de Programacion para Microcontroladores PIC16F.

## Lista de practicas desarrolladas en la Materia
### - P11 - Lectura de Temperatura DS18B [APTEMDS18]
  <p align="center">
  <img src="/images/ap11temds18.png"></img>
</p>
El programa realiza la lectura de tres sensores de temperatura digital DS18B, a intervalos de un segundo, también dispone de un pulsador para hacer lectura del código rom identificador de cada sensor. Los valores de temperatura se enviar por mensaje UART en grados, Utiliza el modulo TMR0 esta configurado para temporizar intervalos de 1ms.(https://youtu.be/kydnK8ixmW8)

### - P12 - Temporizador con Codificador Rotatorio [APROTARYTIMER]
<p align="center">
  <img src="/images/ap12rotarytimer.png"></img>
</p>
El programa utiliza un controlador de segmentos TM1638 con cuatro digitos tipo reloj. Permite el ajustar el tiempo en MM:SS utilizando un encoder rotativo y con un pulsador dar inicio a la cuentra regresiva, el programa utiliza el temporizador TMR0 para generar intervalos de 1ms. (https://youtu.be/c5E6q9o31oM)

### - P13 - Sensor de Humedad y Temperatura en LCD [APDISPLAYDHT]
<p align="center">
  <img src="/images/ap13displaydht.png"></img>
</p>
El programa de este circuito lleva a cabo la lectura de tres sensores DHT Humedad relativa y Temperatura, cuya informacion se visualiza en una Pantalla LCD de 48x84 pixeles basada en controlador PCD8544 (Nokia W5110) utilizado una fuente estandar de 7x5. Los datos del Sensor DHT11 muestran datos enteros mientras que para el modelo DHT22 se presenta el valor entero y decimal. (https://youtu.be/qyUtO6ff_m4) 

### - P14 - Control de LEDS inteligentes WS2812B [APWSLIGHT] *En Desarrollo
<p align="center">
  <img src="/images/ap14wslight.png"></img>
</p>
Se muestra un programa que permite el control de ocho leds inteligentes WS2812B, el cual puede ampliarse con facilidad. Se utiliza un microcontrolador PIC16F gama media operando a 20MHz utlizando instrucciones ASM para calcular los tiempos de señal necesarios. El programa dispone de dos secuencias de luz que pueden seleccionarse a travez del pulsador BUT1.  

Adjunto el siguiente link que muestra como compilar estos ejemplos en MPLABX <br />

[![](http://img.youtube.com/vi/w-GRu89glrg/0.jpg)](http://www.youtube.com/watch?v=w-GRu89glrg "Compilar en MPLABX")


<img src="/images/pablo_ueb.png" align="left" width="200px"/>
Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524)). <br />
Santa Cruz - Bolivia 
<br clear="left"/>
***********************


