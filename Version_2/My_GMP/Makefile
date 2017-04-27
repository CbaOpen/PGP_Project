CFLAGS=-c -g -Wall

run: compil
	./my_gmp

valgrind: compil
	valgrind ./my_gmp

gdb: compil
	gdb my_gmp
	
benchmark: benchmark_compil
	./benchmark

benchmark_compil: benchmark.o my_gmp.o bits.o printf.o
	gcc -Wall -o benchmark benchmark.o my_gmp.o bits.o printf.o
	
compil: main.o my_gmp.o bits.o printf.o
	gcc -Wall -o my_gmp main.o my_gmp.o bits.o printf.o

benchmark.o: benchmark.c
	gcc $(CFLAGS) benchmark.c

main.o: main.c my_gmp.h
	gcc $(CFLAGS) main.c

my_gmp.o: my_gmp.c my_gmp.h
	gcc $(CFLAGS) my_gmp.c

bits.o: bits.c bits.h my_gmp.h
	gcc $(CFLAGS) bits.c

printf.o: printf.c printf.h my_gmp.h
	gcc $(CFLAGS) printf.c

clean:
	rm -f *.o
	rm -f my_gmp

