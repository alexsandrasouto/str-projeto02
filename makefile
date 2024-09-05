# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g

# Bibliotecas necessárias
LIBS = -ljson-c -lm

# Nome do executável
TARGET = scheduler

# Arquivos fonte
SRCS = main.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(TARGET)

# Como criar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra de limpeza
clean:
	rm -f $(OBJS) $(TARGET)

# Regra para rodar o programa
run: $(TARGET)
	./$(TARGET)

