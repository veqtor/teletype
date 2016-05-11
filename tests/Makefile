.PHONY: clean format test
CFLAGS = -std=c99 -g -Wall -DSIM -I../src -I../libavr32/src

tests: main.o parser_tests.o process_tests.o \
	../src/teletype.o ../src/command.o ../src/helpers.o \
	../src/state.o ../src/table.o \
	../src/ops/op.o ../src/ops/constants.o ../src/ops/controlflow.o \
	../src/ops/delay.o ../src/ops/hardware.o ../src/ops/metronome.o \
	../src/ops/maths.o ../src/ops/patterns.o ../src/ops/queue.o \
	../src/ops/stack.o ../src/ops/variables.o \
	../src/euclidean/data.o ../src/euclidean/euclidean.o \
	../libavr32/src/util.o
	$(CC) -o $@ $^ $(CFLAGS)

test: tests
	@./tests | greatest/greenest

clean:
	rm -f tests
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/euclidean/*.o
	rm -f ../src/ops/*.o
	rm -f ../libavr32/src/*.o

format:
	clang-format -style=file -i main.c
	clang-format -style=file -i parser_tests.h
	clang-format -style=file -i parser_tests.c
	clang-format -style=file -i process_tests.h
	clang-format -style=file -i process_tests.c
