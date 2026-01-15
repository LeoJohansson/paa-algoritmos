CC = gcc
CFLAGS = -Wall -O2 -I.
TARGET = tp3_parte2
OBJS = tp3_parte2.o bmh.o huffman.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

tp3_parte2.o: tp3_parte2.c bmh.h huffman.h
	$(CC) $(CFLAGS) -c tp3_parte2.c

bmh.o: bmh.c bmh.h
	$(CC) $(CFLAGS) -c bmh.c

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

clean:
	rm -f $(OBJS) $(TARGET)