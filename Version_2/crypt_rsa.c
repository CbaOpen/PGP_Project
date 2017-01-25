#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "genere_cle.h"


uint64_t exp_rapide(uint64_t c, uint64_t e, uint64_t n)
{
	uint64_t x=1, i=e;
	
	while(i){
		if ((i&1) == 1) x = (x*c)%n; //i%2 == 1
		i = i>>1; //i = i/2
		c = (c*c)%n;
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

//probleme
void decrypt(char* file_crypt, char* file_d, uint64_t d, uint64_t n)
{
	FILE* fc = fopen(file_crypt, "r");
	if(fc==NULL){ printf("FICHIER INEXISTANT\n"); exit(1); }
	FILE* fd = fopen(file_d,"w");
	if(fd==NULL){ printf("FICHIER IMPOSSIBLE A CREER\n"); exit(1); }
	
	long long int c=0;
	uint64_t x;
	
	do{
		fscanf(fc,"%lld ",&c);
		x = exp_rapide(c, d, n);
		fputc(x,fd);
		}while(c != EOF);
}

int main (int argc, char** argv)
{
	uint64_t p,q,n,e,d;
		
	genere_cle(&p, &q, &n, &e, &d);
	
	if (argc == 4){
		/*if (strcmp(argv[1],"-e") == 0)*/ encrypt(argv[2],argv[3],e,n);
		 decrypt(argv[3],"decrypt.txt",d,n);
		}
	
	return 0;
}
