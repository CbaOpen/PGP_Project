#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h"
#include "printf.h"

//fonction d'initialisation d'une variable de type UINT_X_B10
UINT_X_B10 init_uint_b10(int taille){
	int i;
	UINT_X_B10 new;
	new.taille = taille;
	new.tab = malloc(new.taille*sizeof(char));
	if(new.tab == NULL){
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

//libère la mémoire du tableau alloué d'une variable UINT_X_B10
void free_uint_x_b10(UINT_X_B10 n){
	free(n.tab);
	}

//si la taille d'un nombre est plus grande que necessaire, 
//réalloue de la mémoire pour juste le nombre de bits qu'il faut
void ajuste_taille_b10(UINT_X_B10 *n)
{
	int i=n->taille-1, cmp=0;
	while(n->tab[i] == 0) { cmp++; i--; }
	if (cmp > 0)
	{
		n->taille -= cmp;
		char *temp = malloc(n->taille*sizeof(char));
		for(i=0; i<n->taille; i++) temp[i] = n->tab[i];
		free_uint_x_b10(*n);
		n->tab=temp;
	}
}

//calcul la puissance de 2 en fonction du rang du bit, soit 2^(rang_bit-1)
//alloue au départ un tableau de taille arbitraire vallant 1000 cases
UINT_X_B10 pow_2(int rang_bit){
	UINT_X_B10 n;
	
	if (rang_bit-1 == 0){
		n = init_uint_b10(1);
		n.tab[0] = 1;
		return n;
	}
	
	int i, j=0, nb_case = 1;
	char retenue = 0;
	int result = 0;
	n = init_uint_b10(1000);
	n.tab[0] = 1;
	
	for(i=0; i<rang_bit-1; i++){
		for(j=0; j<nb_case; j++)
		{
			result = n.tab[j]*2 + retenue;
			retenue = 0;
			if (result < 10) n.tab[j] = result;
			else
			{
				result -= 10; retenue = 1;
				n.tab[j] = result;
			}
		}
		if(retenue == 1) { n.tab[j] = retenue; retenue = 0; nb_case++; }
	}
	ajuste_taille_b10(&n);
	return n;
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



//écrit la variable de type uint_x en base 10
//attention, cette fonction n'est pas correcte
void printf_b10_uint_x (UINT_X n)
{
	UINT_X_B10 m;
	int i;
	m = pow_2(2048);
	
	for(i=m.taille-1; i>=0; i--){
		printf("%d",m.tab[i]);
	}
	printf("\n");
	free_uint_x_b10(m);
}
