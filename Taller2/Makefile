# Variables
CC = gcc
CFLAGS = -Wall -pthread
TARGET = word_counter
SRC = src/main.c

# Regla predeterminada
all: $(TARGET)

# Regla para compilar el programa
$(TARGET): $(SRC)
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar archivos generados
clean:
    rm -f $(TARGET)