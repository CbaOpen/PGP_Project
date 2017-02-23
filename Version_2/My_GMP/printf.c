#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h"


//affiche un nombre de type uint_x en binaire
void printf_binaire_uint_x(UINT_X n)
{
	int i,j; uint64_t temp;
	
	for(j=n.taille-1;j>=0;j--){
		for(i=64;i>0;i--){
			temp=n.tab[j];
			temp>>=(i-1);
			temp<<=(64-1);
			temp>>=(64-1);
			printf("%"PRIu64,temp);
		}
		printf("\n");
	}
}



//écrit la variable de type uint_x en base 10
//attention, cette fonction n'est pas correcte
void printf_b10_uint_x (UINT_X n){
	int j;
	for(j=n.taille-1;j>=0;j--){
		printf(" %"PRIu64,n.tab[j]);
		
	}
	printf("\n");
}
