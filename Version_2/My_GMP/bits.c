//quelques fonctions sur d'informations sur les bits d'un nombre

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"

//renvoie le nombre de bits d'un nombre de type uint64
int nb_bits(uint64_t a)
{
	int nb=0;
	while(a){ nb++; a = a>>1; }
	return nb;
	
}

//renvoie le bit à un certain rang
int access_bit_n(uint64_t in,int n)
{
	if(n<=0) return -1;
	else if(n>64) return -1;
	in>>=(n-1);
	in<<=(64-1);
	in>>=(64-1);
	return in;
}

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
