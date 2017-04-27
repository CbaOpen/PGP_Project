#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_gmp.h"
#include "printf.h"

#define TAILLE 2
#define NB_TEST 5

int main(){
	float temps;
	clock_t t1, t2;
	srand(time(NULL));
	
	UINT_X a = malloc_uint_x(TAILLE*64);
	UINT_X b = malloc_uint_x(TAILLE*64);
	UINT_X c = malloc_uint_x((TAILLE+1)*64);
	
	t1 = clock();
	for(int j=0; j<NB_TEST; j++){
		for(int i=0; i<TAILLE; i++){
			a.tab[i] = rand()*rand();
			b.tab[i] = rand()*rand();
		}
		somme1(&c, a, b);
		init_uint_x(&c);
	}
	t2 = clock();
	
	temps = (float)(t2-t1)/CLOCKS_PER_SEC;
	printf("temps de somme1 : %f\n", temps);
	
	t1 = clock();
	for(int j=0; j<NB_TEST; j++){
		for(int i=0; i<TAILLE; i++){
			a.tab[i] = rand();
			b.tab[i] = rand();
		}
		somme(&c, a, b);
		init_uint_x(&c);
	}
	t2 = clock();
	
	temps = (float)(t2-t1)/CLOCKS_PER_SEC;
	printf("temps de somme amelioree : %f\n", temps);
	
	free_uint_x(a);
	free_uint_x(b);
	free_uint_x(c);
	
	return 0;
}
