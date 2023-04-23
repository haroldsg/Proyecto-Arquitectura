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

Se hizo un algoritmo capaz de leer los datos de un dataset SGY, se coloca su archivo SGY en la carpeta SGY, se compila el main y luego se ejecuta

### Compilar
g++ main.cpp -o main

### Ejecutar
./main

Se generan 3 archivos txt que son Encabezado, InformacionBinaria y Trazas. Este algoritmo consta de 4 librerias (Aca si quieres pones mas info del algoritmo pero creo que lo mas explicito es en el informe del proyecto cuando se entregue). 

