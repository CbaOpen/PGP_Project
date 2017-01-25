CFLAGS=-c -g -Wall

compil: crypt_rsa.o genere_cle.o
	gcc -Wall -o rsa crypt_rsa.o genere_cle.o -lm

crypt_rsa.o: crypt_rsa.c
	gcc $(CFLAGS) crypt_rsa.c

genere_cle.o: genere_cle.c genere_cle.h
	gcc $(CFLAGS) genere_cle.c -lm
	
clean:
	rm -f *.o
	rm -f rsa

