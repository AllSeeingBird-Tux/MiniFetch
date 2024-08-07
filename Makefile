CC = gcc
CFLAGS = -Wall -Wextra
TARGET = minifetch
OBJFILES = main.o other.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

main.o: main.c other.h
	$(CC) $(CFLAGS) -c main.c

other.o: other.c other.h
	$(CC) $(CFLAGS) -c other.c

clean:
	rm -f *.o $(TARGET)
