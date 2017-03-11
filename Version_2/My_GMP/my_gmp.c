//Fonctions de calculs sur des nombres à taille variable
//somme............ lignes 54 à 148
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"
#include "printf.h"

#define max(a,b) ((a<b)?(b):(a))
#define min(a,b) ((a<b)?(a):(b))


//initialise une variable dont la taille est donnée en argument
UINT_X init_uint_x(int taille){
	UINT_X new;
	new.init = 1;
	new.taille=taille/(8*sizeof(uint64_t));
	new.tab=calloc(new.taille, sizeof(uint64_t));
	if(new.tab==NULL){
		printf("IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(1);
	}
	return new;
}

//libère la variable de type uint_x
void free_uint_x(UINT_X n){
	free(n.tab);
	n.init = 0;
}

//fait la copie d'un UINT_X
//alloque la mémoire qu'il faut donc pas besoin de le faire avant
void copier(UINT_X *copie, UINT_X original)
{
	int i;
	if(copie->taille<original.taille){
		free_uint_x(*copie);
		*copie = init_uint_x(original.taille*64);
		}
	for (i=0; i<original.taille; i++) copie->tab[i] = original.tab[i];
}

//si la taille d'un nombre est plus grande que necessaire, 
//réalloue de la mémoire pour juste le nombre de bits qu'il faut
void ajuste_taille(UINT_X *n)
{
	int i=n->taille-1, cmp=0;
	while(n->tab[i] == 0) { cmp++; i--; }
	if (cmp > 0)
	{
		n->taille -= cmp;
		uint64_t *temp = malloc(n->taille*sizeof(uint64_t));
		for(i=0; i<n->taille; i++) temp[i] = n->tab[i];
		free_uint_x(*n);
		n->tab=temp;
	}
}

//à voir comment on va affecter une valeure à uint_x
UINT_X affect_var(UINT_X n)
{
	
	return n;
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
void somme(UINT_X *resultat, UINT_X a, UINT_X b)
{
	int i;
	char retenue=0;
	
	if(resultat->taille <= max(a.taille,b.taille)) {
		free_uint_x(*resultat);
		*resultat = init_uint_x((1+max(a.taille,b.taille))*64);
		}
	
	for (i=0; i<max(a.taille, b.taille); i++)
	{
		if (i < min(a.taille, b.taille))
		{
			resultat->tab[i] = somme_uint64(a.tab[i], b.tab[i], &retenue);
		}
		else 
		{
			if(max(a.taille, b.taille) == a.taille)
				resultat->tab[i] = somme_uint64(a.tab[i], 0, &retenue);
			else resultat->tab[i] = somme_uint64(b.tab[i], 0, &retenue);
		}
	}
	if(retenue==1) resultat->tab[max(a.taille,b.taille)]=1;
}

//Renvoie original shifté de bit+case_tab*64 dans la variable copie
//case_tab: le nombre de case à shifter
//bit: le nombre de bit à shifter dans la case après case_tab
//1er if: si le nombre de bit à shifter après case_tab = 0 ou que le shifte de bit a déjà été fait et qu'il n'y a plus de chiffre à décaler (temp.tab[i-1-case_tab]) alors le nombre est directement recpier dans copie
//sinon, on copie bit après bit le nombre de temp.tab[i] à partir du rang 64-bit
//temp.tab[i] est ensuite shifté de 64-bit et s'il est encore > 0,
//alors le reste est recopié lors de la boucle suivante dans la case de copie
//le dernier if sert à recopier les derniers bits dans la dernière case de copie
void shift(UINT_X *copie, UINT_X original, int case_tab, int bit)
{
	if(copie->tab != NULL) free_uint_x(*copie);
	*copie = init_uint_x((original.taille+case_tab+1)*64);
	UINT_X temp = init_uint_x(original.taille*64);
	int i,j;
	copier(&temp, original);
	
	for(i=case_tab;i<(temp.taille+case_tab);i++)
	{
		if(bit == 0 || (i>case_tab && temp.tab[i-1-case_tab]==0)) copie->tab[i]=temp.tab[i-case_tab];
		else
		{
			for(j=64-bit; j>0; j--)
			{
				copie->tab[i] <<=1;
				copie->tab[i] += access_bit_n(temp.tab[i-case_tab], j);
				
			}
			temp.tab[i-case_tab] >>= 64-bit;
			if(i==case_tab)	copie->tab[i] <<= bit;
			else
			{
				j=bit;
				while (j > 0)
				{				
					copie->tab[i] <<=1;
					copie->tab[i] += access_bit_n(temp.tab[i-1-case_tab], j);
					j--;
				}
			}
		}
	}
	if(temp.tab[i-1-case_tab] > 0)
	{
		j=bit;
		while (j > 0)
		{				
			copie->tab[i] <<=1;
			copie->tab[i] += access_bit_n(temp.tab[i-1-case_tab], j);
			j--;
		}
	}
	free_uint_x(temp);
}

//multiplication de a par b
//à chaque fois que le bit lu de b vaut 1, on décale a du nombre de bits égale au rang du bit lu de b
//puis on fait la somme avec le résultat précédent
void produit(UINT_X *resultat, UINT_X a, UINT_X b)
{
	UINT_X temp; temp.tab = NULL;
	UINT_X copie = init_uint_x(resultat->taille*64);
	int i,j;
	
	if (resultat->init == 0) *resultat = init_uint_x((a.taille + b.taille)*64);
	else if (resultat->taille < (a.taille + b.taille)){
		free_uint_x(*resultat);
		*resultat = init_uint_x((a.taille + b.taille)*64);
		}
	
	for(i=0; i<b.taille; i++)
	{
		for(j=1;j<=64;j++)
		{
			if (access_bit_n(b.tab[i], j))
			{
				copier(&copie, *resultat);
				shift(&temp,a,i,j-1);
				somme(resultat, copie, temp);
			}
		}
	}
	free_uint_x(temp);
	free_uint_x(copie);
	ajuste_taille(resultat);
}

int gestion_retenue(uint64_t a,uint64_t b,int i,int* retenueInf){
	int temp;
	if(*retenueInf==0){ //si il n'y a pas de retenue
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si la premiere colonne vaut 0 et 0
			temp=0; //PAS DE RETENUE DANS CE CAS
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1
			temp=1;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0
			temp=1;
			*retenueInf=0;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1
			temp=0;
			*retenueInf=0;
		}
	}
	else{ //si il y a une retenue finale
		if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==0)){ // si ca vaut 0 et 0 avec retenue inferieure
			temp=1;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==0)&&(access_bit_n(b,i)==1)){ //si ca vaut 0 et 1 avec retenue inferieure
			temp=0;
			*retenueInf=1;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==0)){ //si ca vaut 1 et 0 avec retenue inferieure
			temp=0;
			*retenueInf=0;
		}
		else if((access_bit_n(a,i)==1)&&(access_bit_n(b,i)==1)){ //si ca vaut 1 et 1 avec retenue inferieure
			temp=1;
			*retenueInf=1;
		}
	}
	return temp;
}

uint64_t difference_uint64(uint64_t a,uint64_t b,int* finalRetenueInf){
	int i,temp;
	uint64_t mask,resultat;
	int retenueInf=0;
	mask=1;resultat=0;
	for(i=1;i<=64;i++){
		if(i==1){
			temp=gestion_retenue(a,b,i,&(*finalRetenueInf));
			
		}
		else {
			temp=gestion_retenue(a,b,i,&retenueInf);
		}
		if(temp==1) {
			resultat|=mask;
		}
		mask<<=1;
	}
	*finalRetenueInf=retenueInf;
	//printf("\n");
	return resultat;
}

UINT_X difference(UINT_X a,UINT_X b){
	UINT_X resultat;
	int i,retenue;
	retenue=0;
	
	resultat=init_uint_x((max(a.taille,b.taille)+1)*64);
	for(i=0;i<(min(a.taille,b.taille));i++){
		resultat.tab[i]=difference_uint64(a.tab[i],b.tab[i],&retenue);
	}
	for(i=(min(a.taille,b.taille));i<max(a.taille,b.taille);i++){
		if(min(a.taille,b.taille)==(a.taille)){
			resultat.tab[i]=difference_uint64(b.tab[i],0,&retenue);
		}
		else resultat.tab[i]=difference_uint64(a.tab[i],0,&retenue);
	}
	if(retenue==1) resultat.tab[max(a.taille,b.taille)]=1;
	else resultat.tab[max(a.taille,b.taille)]=0;
	ajuste_taille(&resultat);
	return resultat;
}

int main(){
	/*UINT_X a=init_uint_x(512-64), b;
	printf("b.init = %d\n",b.init);
	b=init_uint_x(512);
	
	a.tab[0]=84616588;  
	b.tab[0]=70;
	
	//int i; for(i=0;i<8;i++){ b.tab[i] = MAX_UINT64; }
	
	ajuste_taille(&a);
	ajuste_taille(&b);

	//affiche le nombre de cases de 64 bits de la variable et sa valeur en binaire
	printf("a>%d\n",a.taille);
	printf_binaire_uint_x(a);
	printf("b>%d\n",b.taille);
	printf_binaire_uint_x(b);

	//calcule la somme et affiche la valeur
	UINT_X c = init_uint_x((1+max(a.taille,b.taille))*64);
	printf("c>%d\n",c.taille); 
	somme(&c,a,b);
	printf_binaire_uint_x(c);
	printf("\n");
	
	UINT_X d = init_uint_x((a.taille+b.taille)*64);
	produit(&d,b,a);
	printf("d>%d\n",d.taille);
	printf_binaire_uint_x(d);
	
	
	free_uint_x(a);
	free_uint_x(b);
	free_uint_x(c);
	free_uint_x(d);
	*/
	
	UINT_X n = init_uint_x(64);
	printf_b10_uint_x(n);
	free_uint_x(n);
	return 0;
}
