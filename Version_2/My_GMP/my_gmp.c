#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"

#define NB_CAR_UINT64 20
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF



UINT_X init_uint_x(int taille){
	int i;
	UINT_X new;
	new.taille=taille/(8*sizeof(uint64_t));
	new.tab=malloc(new.taille*sizeof(uint64_t));
	if(new.tab==NULL){
		printf("IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(1);
	}
	for(i=0;i<new.taille;i++)
	{
		new.tab[i]=0;
	}
	return new;
}

void free_uint_x(UINT_X n){
	free(n.tab);
}

void printf_uint_x (UINT_X n){
	int j;
	for(j=n.taille-1;j>=0;j--){
		printf(" %"PRIu64,n.tab[j]);
		
	}
	printf("\n");
}

int calcul_retenue(uint64_t mask, uint64_t grand)
{
	
	
	
}

RESULTSOMME64 somme_uint64(uint64_t a,uint64_t b){
	int i, temp;
	int retenue=0;
	uint64_t mask;
	RESULTSOMME64 result;
	mask=1;
	result.resultat=0;
	result.finalRetenue=0;
	
	temp=access_bit_n(a,1)+access_bit_n(b,1);
	for(i=1;i<=64;i++){
		//if(i==1) temp=access_bit_n(a,i)+access_bit_n(b,i)+ result.finalRetenue;
		temp=access_bit_n(a,i)+access_bit_n(b,i)+retenue;
		
		switch(temp){
			case 0:
				retenue=0;
				break;
		
		case 1:
			result.resultat |= mask;
			retenue=0;
			break;
		case 2:
			if(i==64) result.finalRetenue=1; 
			else retenue=1;
			break;
		default:
			if(i==64) result.finalRetenue=1; 
			else retenue=1;
			result.resultat |= mask;
			break;
		}
		mask<<=1;
		
	}/*
	//printf("RETENUE FINALE: %d\n",finalRetenue);
	printf("\n");
	printf("A: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
		}
	printf("\n");
	printf("B: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(b,i));
		}
	printf("\n");
	printf("C: ");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(resultat,i));
		}
	printf("\n");
	printf("RETENUE FINALE: %d\n",finalRetenue);*/
	return result;
}


UINT_X somme(UINT_X c, UINT_X a, UINT_X b)
{
	int taille, autre;
	int i;
	RESULTSOMME64 result;
	uint64_t *grand, *petit;
	
	if (a.taille >= b.taille) {
		taille = a.taille;
		autre = b.taille;
		grand = a.tab;
		petit = b.tab;
		}
	else {
		taille = b.taille;
		autre = a.taille;
		grand = b.tab;
		petit = a.tab;
		}
	
	for (i=0; i<taille; i++)
	{
		if (i < autre)
		{
			result = somme_uint64(grand[i], petit[i]);
			c.tab[i] = result.resultat;
		}
		else c.tab[i] = grand[i];
	if((i+1) < taille) grand[i+1] += result.finalRetenue;
	else c.tab[i+1] += result.finalRetenue;
	}
	return c;
}

int main(){
	int i;
	UINT_X a=init_uint_x(512-64), b=init_uint_x(512);
	
	a.tab[0]=1;
	for(i=0;i<b.taille;i++){ b.tab[i]=999;	}
	printf("a>%d\n",a.taille);
	printf_uint_x(a);
	printf("b>%d\n",b.taille);
	printf_uint_x(b);
	
	UINT_X c = init_uint_x(512+64);
	printf("c>%d\n",c.taille); 
	c = somme(c,a,b);
	printf_uint_x(c);
	printf("\n");
	
	
	free_uint_x(a);
	free_uint_x(b);
	free_uint_x(c);

	/*uint64_t a,b,c;
	a=MAX_UINT64;
	b=69;
	int i;
	//printf("\n");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
	}
	a+=4;
	printf("\n");
	for(i=64;i>0;i--){
		printf("%d",access_bit_n(a,i));
	}
	printf("\n");
	c=somme_uint64(a,b);
	printf("%"PRIu64,c);
	printf("\n");*/
	
	
	return 0;
}
