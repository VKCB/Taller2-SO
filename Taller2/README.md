# Word Counter Project

Este proyecto cuenta la cantidad de apariciones de una palabra específica en el archivo `ElQuijote.txt` utilizando múltiples hilos para realizar el conteo en paralelo. El programa está diseñado para utilizar 2, 3, 4 y 5 hilos, y mide el tiempo de ejecución del conteo.

## Estructura del Proyecto

```
word-counter-project
├── src
│   ├── main.c
├── data
│   └── ElQuijote.txt
├── Makefile
└── README.md
```

## Archivos del Proyecto

- **src/main.c**: Contiene la implementación principal del programa que realiza el conteo de palabras.
- **data/ElQuijote.txt**: Archivo de texto que se analiza para contar las apariciones de la palabra especificada.
- **Makefile**: Instrucciones para compilar el programa y generar el ejecutable.

## Instrucciones para Compilar y Ejecutar

1. **Compilar el Proyecto**:
   Abre una terminal en la raíz del proyecto y ejecuta el siguiente comando:
   ```
   make
   ```

2. **Ejecutar el Programa**:
   Una vez compilado, puedes ejecutar el programa con el siguiente comando:
   ```
   ./word-counter <palabra> <num_hilos>
   ```
   Donde `<palabra>` es la palabra que deseas contar y `<num_hilos>` puede ser 2, 3, 4 o 5.

## Ejemplo de Uso

Para contar la palabra "caballero" utilizando 4 hilos, ejecuta:
```
./word-counter caballero 4
```

## Detalles de Funcionamiento

El programa divide el archivo `ElQuijote.txt` en partes iguales según el número de hilos especificado. Cada hilo cuenta las apariciones de la palabra en su sección asignada y al final, se suman los resultados de todos los hilos. El tiempo total de ejecución se mide y se muestra al final del conteo.