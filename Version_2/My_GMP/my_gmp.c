//Fonctions de calculs sur des nombres à taille variable
//somme............ lignes 54 à 148
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"

#define max(a,b) ((a<b)?(b):(a))
#define min(a,b) ((a<b)?(a):(b))


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

//si la taille d'un nombre est plus grande que necessaire, 
//réalloue de la mémoire pour juste le nombre de bits qu'il faut
void ajuste_taille(UINT_X *n)
{
	int i=(*n).taille-1, cmp=0;
	while((*n).tab[i] == 0) { cmp++; i--; }
	//printf("cmp : %d\n", cmp);
	if (cmp > 0)
	{
		(*n).taille -= cmp;
		uint64_t *temp = malloc((*n).taille*sizeof(uint64_t));
		for(i=0; i<(*n).taille; i++) temp[i] = (*n).tab[i];
		free_uint_x(*n);
		(*n).tab=temp;
	}
}

//à voir comment on va affecter une valeure à uint_x
UINT_X affect_var(UINT_X n)
{
	
	return n;
}

//écrit la variable de type uint_x en base 10
//attention, cette fonction n'est pas correcte
void printf_uint_x (UINT_X n){
	int j;
	for(j=n.taille-1;j>=0;j--){
		printf(" %"PRIu64,n.tab[j]);
		
	}
	printf("\n");
}

//fait la somme en base 2 de deux variables de type uint64
uint64_t somme_uint64(uint64_t a,uint64_t b, char *finalRetenue){
	int i, temp=0;
	char retenue=0;
	uint64_t mask=1, resultat=0;
	
	if (b!=0 || *finalRetenue!=0){
		temp = access_bit_n(a,1) + access_bit_n(b,1) + *finalRetenue;
		*finalRetenue = 0;
		for(i=1;i<=64;i++){
			if(i!=1) temp = access_bit_n(a,i) + access_bit_n(b,i) + retenue;
		
			switch(temp){
			case 0:
				retenue = 0;
				break;
			case 1:
				resultat |= mask;
				retenue = 0;
				break;
			case 2:
				if(i==64) *finalRetenue = 1; 
				else retenue=1;
				break;
			default:
				if(i==64) *finalRetenue = 1; 
				else retenue = 1;
				resultat |= mask;
				break;
				}
			mask<<=1;
		}
	}
	else resultat = a;
	return resultat;
}

//attention, les calculs se font en base 2 donc la représentation en base 10 du nombre de type uint64 n'est pas correcte
//à voir pour créer une fonction permettant d'afficher ce nombre en base 10
//Fait la somme de deux nombres de type uint_x
UINT_X somme(UINT_X resultat, UINT_X a, UINT_X b)
{
	int i;
	char retenue=0;
	
	if(resultat.taille <= max(a.taille,b.taille)) {
		free_uint_x(resultat);
		resultat = init_uint_x((1+max(a.taille,b.taille))*64);
		}
	
	for (i=0; i<max(a.taille, b.taille); i++)
	{
		if (i < min(a.taille, b.taille))
		{
			resultat.tab[i] = somme_uint64(a.tab[i], b.tab[i], &retenue);
		}
		else 
		{
			if(max(a.taille, b.taille) == a.taille)
				resultat.tab[i] = somme_uint64(a.tab[i], 0, &retenue);
			else resultat.tab[i] = somme_uint64(b.tab[i], 0, &retenue);
		}
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	return resultat;
}

//Renvoie original shifté de bit+case_tab*64 dans la variable copie
void egale_shift(UINT_X *copie, UINT_X original, int case_tab, int bit)
{
	if((*copie).tab != NULL) free((*copie).tab);
	*copie = init_uint_x((original.taille+case_tab+1)*64);
	int i,j;
	
	for(i=case_tab;i<(original.taille+case_tab);i++)
	{
		if(bit == 0 || (i>case_tab && original.tab[i-1-case_tab]>0)) (*copie).tab[i]=original.tab[i-case_tab];
		else
		{
			for(j=64-bit; j>0; j--)
			{
				(*copie).tab[i] <<=1;
				(*copie).tab[i] += access_bit_n(original.tab[i-case_tab], j);
			}
			original.tab[i-case_tab] >>= 64-bit;
			if(i==case_tab)	(*copie).tab[i] <<= bit;
			else
			{
				j=bit;
				while (j > 0)
				{				
					(*copie).tab[i] <<=1;
					(*copie).tab[i] += access_bit_n(original.tab[i-1-case_tab], j);
					j--;
				}
			}
		}
	}
	if(original.tab[i-1-case_tab] > 0)
	{
		j=bit;
		while (j > 0)
		{				
			(*copie).tab[i] <<=1;
			(*copie).tab[i] += access_bit_n(original.tab[i-1-case_tab], j);
			j--;
		}
	}
}

//multiplication
UINT_X multiplication(UINT_X resultat, UINT_X a, UINT_X b)
{
	UINT_X temp; temp.tab = NULL;
	UINT_X copie;
	int i,j;
	
	for(i=0; i<b.taille; i++)
	{
		for(j=1;j<=64;j++)
		{
			if (access_bit_n(b.tab[i], j))
			{
				copie = resultat;
				egale_shift(&temp,a,i,j);
				resultat = somme(resultat, copie, temp);
			}
		}
	}
	free_uint_x(temp);
	ajuste_taille(&resultat);
	return resultat;
}

int main(){
	int i;
	UINT_X a=init_uint_x(512-64), b=init_uint_x(512);
	
	a.tab[0]=7; b.tab[0]=111;
	//for(i=0;i<4;i++){ b.tab[i] = MAX_UINT64; }
	
	ajuste_taille(&a);
	ajuste_taille(&b);

	//affiche le nombre de cases de 64 bits de la variable et sa valeur en binaire
	printf("a>%d\n",a.taille);
	printf_binaire_uint_x(a);
	printf("b>%d\n",b.taille);
	printf_binaire_uint_x(b);
/*
	//calcule la somme et affiche la valeur
	UINT_X c = init_uint_x((1+max(a.taille,b.taille))*64);
	printf("c>%d\n",c.taille); 
	c = somme(c,a,b);
	printf_binaire_uint_x(c);
	printf("\n");
	*/
	UINT_X d = init_uint_x((a.taille+b.taille)*64);
	d = multiplication(d,b,a);
	printf("d>%d\n",d.taille);
	printf_binaire_uint_x(d);
	
	
	free_uint_x(a);
	free_uint_x(b);
	//free_uint_x(c);
	free_uint_x(d);
	
	return 0;
}
