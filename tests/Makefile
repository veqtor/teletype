.PHONY: clean format test
CFLAGS = -std=c99 -g -Wall -DSIM -I../src -I../libavr32/src

tests: main.o parser_tests.o process_tests.o \
	../src/teletype.o ../src/table.o ../src/euclidean/data.o ../src/euclidean/euclidean.o \
	../libavr32/src/util.o
	$(CC) -o $@ $^ $(CFLAGS)

test: tests
	@./tests | greatest/greenest

clean:
	rm -f tests
	rm -f *.o
	rm -f ../src/*.o
	rm -f ../src/euclidean/*.o
	rm -f ../libavr32/src/*.o

format:
	clang-format -style=file -i main.c
	clang-format -style=file -i parser_tests.h
	clang-format -style=file -i parser_tests.c
	clang-format -style=file -i process_tests.h
	clang-format -style=file -i process_tests.c
