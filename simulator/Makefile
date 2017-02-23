.PHONY: clean
CFLAGS=-std=c99 -g -Wall -DSIM -I. -I../src -I../libavr32/src
DEPS =
OBJ = tt.o ../src/teletype.o ../src/command.o ../src/helpers.o \
	../src/state.o ../src/table.o \
	../src/ops/op.o ../src/ops/constants.o ../src/ops/controlflow.o \
	../src/ops/delay.o ../src/ops/hardware.o ../src/ops/metronome.o \
	../src/ops/maths.o ../src/ops/patterns.o ../src/ops/queue.o \
	../src/ops/stack.o ../src/ops/telex.o ../src/ops/variables.o \
	../libavr32/src/euclidean/euclidean.o ../libavr32/src/euclidean/data.o \
	../libavr32/src/util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tt: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f tt
	rm -rf tt.dSYM
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/ops/*.o
	rm -f ../libavr32/src/euclidean/*.o
	rm -f ../libavr32/src/*.o
