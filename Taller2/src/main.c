#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_WORD 100

// Estructura para pasar argumentos a los hilos
typedef struct {
    long inicio; 
    long fin;    
    const char *nombre_archivo; 
    const char *palabra;        
    int contador;               
} HiloArgumento;

// Función para limpiar una palabra eliminando caracteres no alfabéticos
void limpiar_palabra(char *palabra) {
    char *src = palabra, *dst = palabra;
    while (*src) {
        // Solo se dejan letras y caracteres especiales como 'ñ'
        if ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src == 'ñ') || (*src == 'Ñ')) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0'; 
}

// Función para convertir una palabra a minúsculas
void convertir_minusculas(char *palabra) {
    for (int i = 0; palabra[i]; i++) {
        // Si es mayúscula, la convertimos a minúscula
        if (palabra[i] >= 'A' && palabra[i] <= 'Z') {
            palabra[i] += 'a' - 'A';
        }
    }
}

// Función que ejecutará cada hilo para contar las palabras en su bloque
void *contar_palabra_parte(void *arg) {
    HiloArgumento *data = (HiloArgumento *)arg;

    
    FILE *archivo = fopen(data->nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir archivo en hilo");
        pthread_exit(NULL);
    }

    
    fseek(archivo, data->inicio, SEEK_SET);

    char palabra_archivo[MAX_WORD];
    long posicion = data->inicio;

    
    if (data->inicio != 0) {
        while (fgetc(archivo) != ' ' && ftell(archivo) < data->fin);
    }

    // Leer palabras dentro del bloque asignado
    while (posicion < data->fin && fscanf(archivo, "%99s", palabra_archivo) == 1) {
        posicion = ftell(archivo);
        limpiar_palabra(palabra_archivo); 
        convertir_minusculas(palabra_archivo); 
        if (strcmp(palabra_archivo, data->palabra) == 0) {
            data->contador++; // Incremento del contador si hay coincidencia
        }
    }

    fclose(archivo); // Cerrar el archivo
    pthread_exit(NULL); // Termina el hilo
}

// Función para obtener el tamaño del archivo
long obtener_tamano_archivo(const char *nombre) {
    FILE *archivo = fopen(nombre, "r");
    if (!archivo) return -1; // Retorna -1 si no se puede abrir el archivo
    fseek(archivo, 0, SEEK_END); 
    long tamano = ftell(archivo); 
    fclose(archivo);
    return tamano;
}

// Función principal para contar palabras usando múltiples hilos
void contar_palabra_con_hilos(const char *nombre_archivo, const char *palabra, int num_hilos) {
    long tamano = obtener_tamano_archivo(nombre_archivo);
    if (tamano <= 0) {
        printf("No se pudo determinar el tamaño del archivo.\n");
        return;
    }

    char palabra_normalizada[MAX_WORD];
    strncpy(palabra_normalizada, palabra, MAX_WORD);
    convertir_minusculas(palabra_normalizada); // Normalizamos la palabra a buscar

    pthread_t hilos[num_hilos]; // Arreglo de identificadores de hilos
    HiloArgumento args[num_hilos]; // Arreglo de argumentos para los hilos

    long bloque = tamano / num_hilos; // Dividimos el archivo en bloques iguales

    clock_t inicio = clock(); // Inicia el cronómetro

    // Creaciòn de los hilos
    for (int i = 0; i < num_hilos; i++) {
        args[i].inicio = i * bloque;
        args[i].fin = (i == num_hilos - 1) ? tamano : (i + 1) * bloque; // Último hilo toma hasta el final
        args[i].nombre_archivo = nombre_archivo;
        args[i].palabra = palabra_normalizada;
        args[i].contador = 0;

        pthread_create(&hilos[i], NULL, contar_palabra_parte, &args[i]);
    }

    int total = 0;
    
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
        total += args[i].contador; 
    }

    clock_t fin = clock(); 
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    // Imprimir  los resultados
    printf("Con %d hilos: La palabra '%s' aparece %d veces.\n", num_hilos, palabra, total);
    printf("Tiempo de ejecución: %.4f segundos\n", tiempo);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s archivo.txt palabra\n", argv[0]);
        return 1;
    }

    const char *nombre_archivo = argv[1];
    const char *palabra = argv[2];

    // Prueba con 2, 3, 4 y 5 hilos
    int hilos[] = {2, 3, 4, 5};
    for (int i = 0; i < 4; i++) {
        contar_palabra_con_hilos(nombre_archivo, palabra, hilos[i]);
    }

    return 0;
}