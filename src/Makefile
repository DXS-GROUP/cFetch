# Устанавливаем компилятор
CC = gcc

# Устанавливаем флаги компилятора
CFLAGS = -Wall -Wextra -g

# Указываем исполняемый файл
TARGET = cfetch

# Указываем исходные файлы
SRCS = main.c \
       get_data/get_cpu.c \
       get_data/gpu_info.c \
       get_data/get_sys.c \
       get_data/get_ram.c \
       get_data/disk_info.c \
	   get_data/uptime.c

# Указываем объектные файлы
OBJS = $(SRCS:.c=.o)

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Правило для сборки объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки скомпилированных файлов
clean:
	rm -f $(OBJS) $(TARGET)

# Правило для запуска программы
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
