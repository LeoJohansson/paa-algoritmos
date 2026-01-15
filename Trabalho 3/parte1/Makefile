CC = gcc
CFLAGS = -Wall -O2 -I.
LDFLAGS = -lm
TARGET = tp3_parte1
OBJS = prog_dinamica.o shift_and.o tp3_parte1.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

prog_dinamica.o: prog_dinamica.c prog_dinamica.h
	$(CC) $(CFLAGS) -c prog_dinamica.c

shift_and.o: shift_and.c shift_and.h
	$(CC) $(CFLAGS) -c shift_and.c

tp3_parte1.o: tp3_parte1.c prog_dinamica.h shift_and.h
	$(CC) $(CFLAGS) -c tp3_parte1.c

clean:
	rm -f $(OBJS) $(TARGET)