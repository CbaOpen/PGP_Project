#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"

int nb_bits(uint64_t a)
{
	int nb=0;
	while(a){ nb++; a = a>>1; }
	return nb;
	
}

int access_bit_n(uint64_t in,int n)
{
	if(n<=0) return -1;
	else if(n>64) return -1;
	in>>=(n-1);
	in<<=(64-1);
	in>>=(64-1);
	return in;
}

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
