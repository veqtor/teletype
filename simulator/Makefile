.PHONY: clean format
CFLAGS=-std=c99 -g -Wall -DSIM -I. -I../src -I../libavr32/src
DEPS =
OBJ = tt.o ../src/teletype.o ../src/command.o ../src/helpers.o \
	../src/state.o ../src/table.o \
	../src/ops/op.o ../src/ops/constants.o ../src/ops/controlflow.o \
	../src/ops/delay.o ../src/ops/hardware.o ../src/ops/metronome.o \
	../src/ops/maths.o ../src/ops/patterns.o ../src/ops/queue.o \
	../src/ops/stack.o ../src/ops/variables.o \
	../src/euclidean/euclidean.o ../src/euclidean/data.o \
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
	rm -f ../src/ops/*.o
	rm -f ../libavr32/src/*.o

format:
	clang-format -style=file -i tt.c
