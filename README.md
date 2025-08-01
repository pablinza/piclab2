# PICMCU LAB2 pablinza@me.com
Programacion del Microcontrolador PIC16F887 LAB2, corresponde a la segunda parte del repositorio con carpetas de los proyectos generados por MPLABX utilizando el compilador XC8 y el PIC16F887. Estos programaas fueron elaborados en la actividad practica de la materia Microcontroladores PIC <br />
El software __MPLABX__ disponible en la pagina del fabricante Microchip [ --> Click](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/MPLABX-v6.20-windows-installer.exe?authuser=0) <br />
El compilador __XC8__ puedes descargalo utilizando este enlace [ --> Click](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8) <br />

Para cargar el firmware al microcontrolador necesitaras un programador ICSP, como alternativa se utiliza el software __SimulIDE__ [ -->Click](https://simulide.com/p/) a efectos de verificar el funcionamiento. Cada carpeta del proyecto MPLABX tiene el nombre precedido por el numero de actividad y en su estructura encontrara el programa principal con el nombre __main.c__ y librerias de uso local, una vez compilado el codigo fuente, se generara el firmware archivo __.hex__ en la carpeta dist/default/production. <br />

Utilizaremos la placa B8P40 que lleva un Cristal HS de 20MHz, un LED, Pulsador Reset y Puerto para comunicacion UART, en todos los casos la programacion del PIC16F887 sera utilizando el oscilador interno RC a 8MHz, salvo casos donde se requiera mas velocidad se usara el HS a 20MHz. En caso de utilizar un __bootloader__, se debe reservar el uso de memoria en MPLABX. <br />

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
El programa utiliza un controlador de segmentos TM1637 con cuatro digitos tipo reloj. Permite el ajustar el tiempo en MM:SS utilizando un encoder rotativo y con un pulsador dar inicio a la cuentra regresiva, el programa utiliza el temporizador TMR0 para generar intervalos de 1ms. (https://youtu.be/c5E6q9o31oM)

### - P13 - Sensor de Humedad y Temperatura en LCD [APDISPLAYDHT]
<p align="center">
  <img src="/images/ap13displaydht.png"></img>
</p>
El programa de este circuito lleva a cabo la lectura de tres sensores DHT Humedad relativa y Temperatura, cuya informacion se visualiza en una Pantalla LCD de 48x84 pixeles basada en controlador PCD8544 (Nokia W5110) utilizado una fuente estandar de 7x5. Los datos del Sensor DHT11 muestran datos enteros mientras que para el modelo DHT22 se presenta el valor entero y decimal. (https://youtu.be/qyUtO6ff_m4) 

### - P14 - Celda de Carga con PGA HX711 [APLOADCELL]
<p align="center">
  <img src="/images/ap14loadcell.png"></img>
</p>
Este circuito realiza la lectura de una celda de carga utilizando un amplificador HX711 y pantallas de siete segmentos modular LTC-5623 tipo anodo comun, el programa reside en un PIC16F887 que recibe los datos digitales y determina la relacion de gramos por fuerza en el rango 0000 - 5000 gr. Es necesario efectuar una medicion referencial con peso conocido por unica vez a cada celda. (https://youtu.be/Iq21xI10iW8) 

### - P15 - Control de LEDS inteligentes WS2812B [APWSLIGHT] *En Desarrollo
<p align="center">
  <img src="/images/ap15wslight.png"></img>
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


