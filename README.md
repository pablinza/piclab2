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

Los proyectos se crearon con la version MPLABX 6.25 y el compilador XC8 3.0 version FREE
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

### - P13 - Sensor de Humedad y Temperatura en LCD [APDHTNOKIA] *En Desarrollo
<p align="center">
  <img src="/images/ap13dhtnokia.png"></img>
</p>
Este programa muestra como utilizar la Pantalla Nokia W5110 84x48 pixeles utlizando fuente estaandar de 12x5 pixels. Se muestra en la pantalla el valor de temperatura y humedad relativa proveniente de un sensor dual DHT-11. 

Adjunto el siguiente link que muestra como compilar estos ejemplos en MPLABX <br />
[![](http://img.youtube.com/vi/w-GRu89glrg/0.jpg)](http://www.youtube.com/watch?v=w-GRu89glrg "Compilar en MPLABX")


<img src="/images/pablo_ueb.png" align="left" width="200px"/>
Autor: Pablo Zarate, puedes contactarme a pablinza@me.com / pablinzte@gmail.com.  <br />
Visita mi Blog  [Blog de Contenidos](https://pablinza.blogspot.com/). <br />
Visita mi Canal [Blog de Contenidos](http://www.youtube.com/@pablozarate7524)). <br />
Santa Cruz - Bolivia 
<br clear="left"/>
***********************


