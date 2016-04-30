.PHONY: clean format
CFLAGS=-std=c99 -g -Wall -DSIM -I. -I../src -I../libavr32/src
DEPS =
OBJ = tt.o ../src/teletype.o ../src/state.o \
	../src/table.o ../src/euclidean/euclidean.o ../src/euclidean/data.o \
	../libavr32/src/util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tt: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f tt
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/euclidean/*.o
	rm -f ../libavr32/src/*.o

format:
	clang-format -style=file -i tt.c
