//Fonctions de calculs sur des nombres à taille variable
//somme............ lignes 54 à 148
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"


//initialise une variable dont la taille est donnée en argument
UINT_X init_uint_x(int taille){
	int i;
	UINT_X new;
	new.taille=taille/(8*sizeof(uint64_t));
	new.tab=malloc(new.taille*sizeof(uint64_t));
	if(new.tab==NULL){
		printf("IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(1);
	}
	//initialise la variable à zéro
	for(i=0;i<new.taille;i++)
	{
		new.tab[i]=0;
	}
	return new;
}

//libère la variable de type uint_x
void free_uint_x(UINT_X n){
	free(n.tab);
}

//à voir comment on va affecter une valeure à uint_x
UINT_X affect_var(UINT_X n)
{
	
	return n;
}

//écrit la variable de type uint_x en base 10
void printf_uint_x (UINT_X n){
	int j;
	for(j=n.taille-1;j>=0;j--){
		printf(" %"PRIu64,n.tab[j]);
		
	}
	printf("\n");
}

//fait la somme en base 2 de deux variables de type uint64
//renvoit un structure comprenant le résultat de la somme et l'éventuelle retenue
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
	}
	return result;
}

//attention, les calculs se font en base 2 donc la représentation en base 10 du nombre de type uint64 n'est pas correcte
//à voir pour créer une fonction permettant d'afficher ce nombre en base 10
//Fait la somme de deux nombres de type uint_x
//les pointeurs sur uint64 évitent de faire trop de tests
UINT_X somme(UINT_X c, UINT_X a, UINT_X b)
{
	int taille_grd, taille_pt;
	int i,j;
	RESULTSOMME64 result;
	uint64_t *grand, *petit;
	
	if (a.taille >= b.taille) 
	{
		taille_grd = a.taille;
		taille_pt = b.taille;
		grand = a.tab;
		petit = b.tab;
	}
	else 
	{
		taille_grd = b.taille;
		taille_pt = a.taille;
		grand = b.tab;
		petit = a.tab;
	}
	
	for (i=0; i<taille_grd; i++)
	{
		if (i < taille_pt)
		{
			result = somme_uint64(grand[i], petit[i]);
			//printf("resultat: %"PRIu64, result.resultat);
			//printf("\nretenue: %d\n", result.finalRetenue);
			c.tab[i] = result.resultat;
			j = i+1;
			while(result.finalRetenue)
			{
				if((j) < taille_grd)
				{
					result = somme_uint64(grand[j], result.finalRetenue);
					grand[j] = result.resultat;
					j++;
				}
				else 
				{
					result = somme_uint64(c.tab[j], result.finalRetenue);
					c.tab[j] = result.resultat;
					j++;
				}
			}
		}
		else c.tab[i] = grand[i];
	
	}
	return c;
}

int main(){
	int i;
	UINT_X a=init_uint_x(512-64), b=init_uint_x(512);
	
	a.tab[0]=10; 
	for(i=0;i<4;i++){ b.tab[i]=MAX_UINT64; }
	
	//affiche le nombre de cases de 64 bits de la variable et sa valeur en binaire
	printf("a>%d\n",a.taille);
	printf_binaire_uint_x(a);
	printf("b>%d\n",b.taille);
	printf_binaire_uint_x(b);
	
	//calcule la somme et affiche la valeur
	UINT_X c = init_uint_x(512+64);
	printf("c>%d\n",c.taille); 
	c = somme(c,a,b);
	printf_binaire_uint_x(c);
	printf("\n");
	
	
	free_uint_x(a);
	free_uint_x(b);
	free_uint_x(c);
	
	return 0;
}
