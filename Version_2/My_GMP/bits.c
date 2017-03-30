//quelques fonctions sur d'informations sur les bits d'un nombre

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"

//renvoie le nombre de bits d'un nombre de type uint64
int nb_bits_uint64(uint64_t a)
{
	int nb=0;
	while(a){ nb++; a = a>>1; }
	return nb;
	
}

int nb_bits_uint_x(UINT_X a)
{
	int nb=0,i;
	for(i=0;i<a.taille;i++) nb += nb_bits_uint64(a.tab[i]);
	return nb;
}

//renvoie le bit à un certain rang
//le premier bit est au rang 1
char access_bit_n(uint64_t a,int n)
{
	if(n<=0 || n>64) return -1;
	a>>=(n-1);
	return a&1;
}
