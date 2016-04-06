CC=gcc
CFLAGS=-g -Wall -I. -I../src -I../../libavr32/src -DSIM
DEPS =
OBJ = tt.o ../src/teletype.o ../src/table.o \
	../src/euclidean/euclidean.o ../src/euclidean/data.o \
	../libavr32/src/util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tt: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o
	rm ../src/*.o
	rm ../src/euclidean/*.o
	rm ../libavr32/src/*.o
