#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "genere_cle.h"


int nb_bits(uint64_t a)
{
	int nb=0;
	while(a){ nb++; a = a>>1; }
	return nb;
	
}

//calcul de c puissance e mod n
uint64_t exp_rapide(uint64_t c, uint64_t e, uint64_t n)
{
	uint64_t x=1;
		
	while(e){
		c=c%n;
		//printf("\nle nb de bits de x mod nc est: %d\n",nb_bits((x%n)*c));
		if ((e&1) == 1) x = ((x%n)*c)%n; //i%2 == 1
		e = e>>1; //i = i/2
		//printf("le nb de bits de c*c est: %d\n",nb_bits(c*c));
		c = (c*c)%n;
		//printf("le nb de bits de c est: %d\n",nb_bits(c));
		//printf("le nb de bits de x est: %d\n",nb_bits(x));
		}
	return x;
}

void encrypt(char* file_e, char* file_crypt, uint64_t e, uint64_t n)
{
	FILE* fe = fopen(file_e, "r");
	if(fe==NULL){ printf("FICHIER INEXISTANT\n"); exit(1); }
	FILE* fc = fopen(file_crypt,"w");
	if(fc==NULL){ printf("FICHIER IMPOSSIBLE A CREER\n"); exit(1); }
	
	uint64_t c;
	uint64_t x;
	
	do{
		c = fgetc(fe);
		x = exp_rapide(c, e, n);
		fprintf(fc,"%llu ",(long long)x);
		}while(c != EOF);
	fclose(fc);
	fclose(fe);
} 

void decrypt(char* file_crypt, char* file_d, uint64_t d, uint64_t n)
{
	FILE* fc = fopen(file_crypt, "r");
	if(fc==NULL){ printf("FICHIER INEXISTANT\n"); exit(1); }
	FILE* fd = fopen(file_d,"w");
	if(fd==NULL){ printf("FICHIER IMPOSSIBLE A CREER\n"); exit(1); }
	
	long long unsigned int c=0;
	uint64_t x;
	
	do{
		c = fgetc(fc);
		if (c != EOF){
			ungetc(c,fc);
			fscanf(fc,"%llu ",&c);
			x = exp_rapide(c, d, n);
			fputc(x,fd);
			}
		}while(c != EOF);
}

// ./rsa [-e|-d] [fichier à chiffrer/dechiffré] [fichier pour le résultat]
int main (int argc, char** argv)
{
	uint64_t p,q,n,e,d;
		
	genere_cle(&p, &q, &n, &e, &d);
	
	if (argc == 4){
		/*if (strcmp(argv[1],"-e") == 0)*/ encrypt(argv[2],argv[3],e,n);
		/*if (strcmp(argv[1],"-d") == 0)*/ decrypt(argv[3],"decrypt.txt",d,n);
		}
	
	return 0;
}
