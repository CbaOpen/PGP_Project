#include <stdio.h>
#include <stdlib.h>
#include "my_gmp.h" 
#include "bits.h"
#include "printf.h"
#include <time.h>

#define TAILLE 5
#define NB_TEST 5


int main(){
	srand(time(NULL));
	UINT_X a = malloc_uint_x(TAILLE*64), b = malloc_uint_x(TAILLE*64);
	UINT_X c = malloc_uint_x((a.taille+b.taille +1)*64);
	for(int j=0; j<NB_TEST; j++){
		for(int i=0; i<TAILLE; i++){
			b.tab[i] = rand()+rand()<<32;
			a.tab[i] = rand()+rand()<<32;
			}
		
		//affiche le nombre de cases de 64 bits de la variable et sa valeur en binaire
		printf_binaire_uint_x(a);
		printf("\n");
		printf_binaire_uint_x(b);
		printf("\n");
		//calcule la somme/produit et affiche la valeur
		if(produit(&c,a,b) == -1) return -1;
		printf_binaire_uint_x(c);
		init_uint_x(&c);
		printf("\n-------------------------------------------------\n");
	}
	/*a.tab[0] = 0xFFDBA456DECB2864;
	b.tab[0] = 0xDABCFE789EBACF39;
	produit(&c,a,b);*/
	/*
	UINT_X d = init_uint_x((a.taille+b.taille)*64);
	produit(&d,b,a);
	printf("d>%d\n",d.taille);
	printf_binaire_uint_x(d);
	*/
	
	free_uint_x(a);
	free_uint_x(b);
	free_uint_x(c);
	//free_uint_x(d);
	
	
	/*UINT_X n = init_uint_x(64);
	printf_b10_uint_x(n);
	free_uint_x(n);*/
	return 0;
}
