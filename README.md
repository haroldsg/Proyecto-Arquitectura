# Proyecto Arquitectura-Primera entrega
## Trabajo hecho por: 
>Harold Franco 
>CI: 28.330.946

>Moises Merza
>CI: 25.547.115

## Leer y cargar un dataset SGY

### ¿Que es un dataset SGY?

El formato de archivo SGY (Seismic Unix) es un formato binario utilizado para almacenar datos sísmicos en la industria geofísica,
la información almacenada en un archivo SGY incluye datos de onda sísmica, información de encabezado y datos de traza.

El encabezado de un archivo SGY contiene información sobre el tamaño de muestra, la frecuencia de muestreo, la profundidad, la fecha de adquisición y otras propiedades relevantes de los datos sísmicos. Los datos de traza contienen información sobre la forma de onda sísmica registrada en un punto de adquisición específico.

## Programa

Se hizo un algoritmo capaz de leer los datos de un dataset SGY, Tenemos 2 carpetas una llamada LIB donde se encuentran las librerías que nos ayudan a leer el dataset y otra llamada SGY donde se coloca el dataset SGY se coloco 3 dataset con su nombre apropiados “DataSet1” es el caso pequeño, “DataSet2” es el caso intermedio y “DataSet3” el caso grande. Colocamos 2 main donde se puede compilar el programa, el “mainMenu.cpp” tiene un menú donde puede escoger entre 3 datasets están nombrados como caso pequeño, intermedio y grande, se leen esos 3 datasets como se menciono anteriormente con sus nombres apropiados y el “main.cpp” se encarga de leer directamente el data set con su nombre apropiado “DataSet1”.

Al final de ejecutar el programa en ambos mains se generara 2 archivos.txt, Encabeza.txt e InformacionBinaria.txt y las trazas se mostraran en pantalla.


### Compilar

Para compilar el "main.cpp"

g++ main.cpp -o main

Para compilar el "mainMenu.cpp"

g++ mainMenu.cpp -o mainMenu

### Ejecutar
Ejecutar el "main.cpp"
./main

Ejecutar el "mainMenu.cpp"
./mainMenu
 

