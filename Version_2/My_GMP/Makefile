CFLAGS=-c -g -Wall

all: compil
	./my_gmp

compil: my_gmp.o bits.o printf.o
	gcc -Wall -o my_gmp my_gmp.o bits.o printf.o

my_gmp.o: my_gmp.c my_gmp.h
	gcc $(CFLAGS) my_gmp.c

bits.o: bits.c bits.h my_gmp.h
	gcc $(CFLAGS) bits.c

printf.o: printf.c printf.h my_gmp.h
	gcc $(CFLAGS) printf.c

clean:
	rm -f *.o
	rm -f my_gmp

