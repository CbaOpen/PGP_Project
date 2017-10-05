/* Fonctions de calculs sur des nombres à taille variable
 * Somme............ ligne 300
 *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "my_gmp.h" 
#include "bits.h"
#include "printf.h"



/* Alloue et initialise une variable dont la taille (nombre de bits) est donnée en argument */
UINT_X malloc_uint_x(int taille){
	UINT_X new;
	new.taille = taille/(8*sizeof(uint64_t));
	new.tab = calloc(new.taille, sizeof(uint64_t));
	if(new.tab==NULL){
		fprintf(stderr, "IMPOSSIBLE DE RESERVER DE LA MEMOIRE POUR UINT_X\n");
		exit(EXIT_FAILURE);
	}
	return new;
}

/* Initialise une variable UINT_X à 0 */
void init_uint_x(UINT_X *n){
	memset(n->tab, 0, n->taille*8);
}

/* Libère la variable de type uint_x */
void free_uint_x(UINT_X n){
	free(n.tab);
}

/* Fait la copie d'un UINT_X
 * il faut que copie ait été initialisé et avec la bonne taille
 * Return value : on success return 1, on failure return -1 */
int copier(UINT_X *copie, UINT_X original)
{
	int i;
	if(copie->tab == NULL || copie->taille < original.taille)
		return fprintf(stderr,"copie pas initialisée ou de taille inférieure à original\n"), -1;
	for (i=0; i<original.taille; i++) copie->tab[i] = original.tab[i];
	return 1;
}

/* Si la taille d'un nombre est plus grande que necessaire, 
 * réalloue de la mémoire pour juste le nombre de bits qu'il faut */
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
	
//renvoie 1 si a<b
int inferieur(UINT_X a,UINT_X b){
	int i;
	if(a.taille<b.taille) return 1;
	else if(a.taille==b.taille){
		for(i=a.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>=b.tab[i]) return 0;
		}	
	}
	else if(a.taille>b.taille){
		for(i=a.taille-1;i>=b.taille-1;i--){
			if(a.tab[i]!=0) return 0;
		}
		for(i=b.taille-1;i>=0;i--){
			if(a.tab[i]<b.tab[i]) return 1;
			if(a.tab[i]>=b.tab[i]) return 0;
		}
		
	}
	return 2;
}

//renvoie 1 si a=b
int egal(UINT_X a, UINT_X b){
	int i;
	int n = min(a.taille, b.taille);
	for(i=0; i<n; i++)
		if(a.tab[i] != b.tab[i]) return -1;
	if(n == a.taille)
		{
		for(i = n; i<b.taille; i++)
			if(b.tab[i] != 0) return -1;
		}
	else
		{
		for(i = n; i<a.taille; i++)
			if(a.tab[i] != 0) return -1;
		}
	return 1;
}

//Fait la somme en base 2 de deux variables de type uint64 
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
void somme1(UINT_X *resultat, UINT_X a, UINT_X b)
{
	int i;
	char retenue=0;
	
	if(resultat->taille <= max(a.taille,b.taille)) {
		free_uint_x(*resultat);
		*resultat = malloc_uint_x((1+max(a.taille,b.taille))*64);
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
	
	resultat=malloc_uint_x((max(a.taille,b.taille)+1)*64);
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

/////////***********************FONCTIONS AMELIOREES*******************///////////

/* ** SOMME **
 * Il faut que resultat soit initialisé resultat->taille >= max(a.taille, b.taille) + 1.
 * Return value : on success return 0, on failure return -1
 * 
 * Lors du calcul de la retenue avec la valeur du bit de poid fort de resultat
 * car si la retenue est > 4, la vraie valeur de la retenue est difficile à avoir 
 * donc on fait retenue + 1 puis on shift pour avoir sa bonne valeur 
 * de meme pour les retenue paires, ret de 10 vaut en fait 1 pour le bloc suivant 
 * exemple: res = 10001 et ret = 101
 * --> res = 00001 et ret = 101+1 = 110 et non ret = 101-10 
 * (10 n'étant pas la même valeur à chaque fois)
 * --> res = 11 00001 */
int somme (UINT_X *resultat, UINT_X a, UINT_X b){
	if(resultat->tab == NULL || resultat->taille < max(a.taille, b.taille))
		return fprintf(stderr, "variable resultat non initialisee ou la taille est inférieure à max(a.taille, b.taille)\n"), -1;
	if((resultat->taille == a.taille && a.tab[a.taille-1] != 0) && (resultat->taille == b.taille && b.tab[b.taille-1] != 0))
		return fprintf(stderr, "la taille est inférieure à max(a.taille, b.taille) + 1\n"), -1;
 
	char retenue = 0; 
	int i;
	
	/* Copie de a et b */
	UINT_X copieA = malloc_uint_x(max(a.taille, b.taille)*64);
	UINT_X copieB = malloc_uint_x(max(a.taille, b.taille)*64);
	if(copier(&copieA, a) == -1) return -1;
	if(copier(&copieB, b) == -1) return -1;
	
	init_uint_x(resultat);
	
	for(i=0; i < copieA.taille; i++){
		/* on ajoute la retenue dans a ou b s'ils sont < MAX_UINT64 
		 * sinon on garde la retenue
		 * la retenue est shifté vers la gauche car le bit de poids faible correcpond
		 * au bit de poids fort du bloc précédent. */
		if((retenue > 0) && (copieA.tab[i] < MAX_UINT64)) {
			copieA.tab[i] += retenue>>1;
			retenue = 0;
		}
		else if((retenue > 0) && (copieB.tab[i] < MAX_UINT64)) {
			copieB.tab[i] += retenue;
			retenue = 0;
		}
		/* on récupère les deux bits de poids fort */
		retenue = access_bit_n(copieA.tab[i], 64) + access_bit_n(copieB.tab[i], 64);
		/* on met les deux bits de poids fort à zéro */
		copieA.tab[i] <<= 1; copieA.tab[i] >>= 1;
		copieB.tab[i] <<= 1; copieB.tab[i] >>= 1;
		resultat->tab[i] = copieA.tab[i] + copieB.tab[i];
		
		/* CAS : retenue paire --> Rien
		 * CAS : retenue impaire -->
		 ** si le bit de poids fort de resultat.tab[i]==0, le bit est mis à 1 
		 * et la retenue diminue de 1
		 ** si le bit de poids fort de resultat.tab[i]==1, le bit est mis à zéro 
		 * et la retenue augmente de 1 */
		if(retenue%2){
			if(access_bit_n(resultat->tab[i], 64) == 0){
				resultat->tab[i] |= 0x8000000000000000;
				retenue--;
			}
			else {
				retenue++;
				resultat->tab[i] <<= 1; resultat->tab[i] >>= 1;
			}
		}
	}
	/* s'il reste une retenue, elle est ajoutée au dernier bloc */
	if(retenue > 0) resultat->tab[i] += retenue>>1;
	free_uint_x(copieA);
	free_uint_x(copieB);	
	
	return 0;
} 

/* Shift une variable UINT_X de nb_block, soit nb_block*64 bits.
 * Retourne -1 sur une erreur, 0 si réussite */
int shift(UINT_X *res, int nb_block){
	UINT_X copie = malloc_uint_x(res->taille*64);
	if(copier(&copie, *res) == -1) return -1;
	ajuste_taille(&copie);
	
	if(res->taille < (copie.taille+nb_block))
		return fprintf(stderr, "impossible de shifter, taille de res trop petite\n"), -1;
		
	init_uint_x(res);
	for(int i = 0; i < copie.taille; i++){
		res->tab[i+nb_block] = copie.tab[i];
	}
	return 0;
}

/* a0 --> partie droite du nombre blk_A partie gauche du nombre
 * même chose pour b0 et blk_B 
 * res = blk_A*blk_B * 2^64 + a0*b0 + (blk_A*b0 + blk_B*a0) * 2^32 */
void block_X_block(UINT_X *res, UINT_X tmp, uint64_t blk_A, uint64_t blk_B){
	uint64_t a0, b0;
	
	/* Initialisation */
	a0 = blk_A;
	blk_A >>= 32;
	a0 <<= 32; a0 >>=32;
	
	b0 = blk_B;
	blk_B >>= 32;
	b0 <<= 32; b0 >>=32;
	
	tmp.tab[0] = a0 * b0;
	tmp.tab[1] = blk_A * blk_B;
	res->tab[0] = blk_A * b0;
	int ret = access_bit_n(res->tab[0], 64);
	res->tab[0] <<= 1; res->tab[0] >>= 1;
	res->tab[0] += blk_B * a0;
	if(ret){
		if(access_bit_n(tmp.tab[0], 64)){
			res->tab[0] <<= 1; res->tab[0] >>= 1;
			res->tab[1] = 1;
		}
		else res->tab[0] |= 0x8000000000000000;
	}
	a0 = tmp.tab[0];
	res->tab[0] <<= 32;
	a0 >>= 32;
	res->tab[1] <<= 33;
	res->tab[1] |= a0;
	somme(res, *res, tmp);
	
}

/* ** PRODUIT **
 * 
 * */
int produit(UINT_X *res, UINT_X a, UINT_X b){
	if(res->tab == NULL || res->taille < (a.taille + b.taille+1))
		return fprintf(stderr, "variable resultat non initialisee ou la taille est inférieure à a.taille + b.taille +1\n"), -1;
	
	/* Copie de a et b */
	UINT_X copieA = malloc_uint_x(a.taille*64);
	UINT_X copieB = malloc_uint_x(b.taille*64);
	if(copier(&copieA, a) == -1) return -1;
	if(copier(&copieB, b) == -1) return -1;
	
	init_uint_x(res);
	UINT_X tmp = malloc_uint_x(res->taille*64);
	UINT_X tmp2 = malloc_uint_x(128);
	uint64_t blockA, blockB;
	
	for(int i=0; i<b.taille; i++){
		blockB = copieB.tab[i];
		for(int j=0; j<a.taille; j++){
			blockA = copieA.tab[j];
			block_X_block(&tmp, tmp2, blockA, blockB);
			if(shift(&tmp, i+j) == -1) return -1;
			if(somme(res,*res,tmp) == -1) return -1;
			init_uint_x(&tmp);
			init_uint_x(&tmp2);
		}
	}
	
	free_uint_x(copieA);
	free_uint_x(copieB);	
	free_uint_x(tmp);
	free_uint_x(tmp2);
	return 0;
}

/* ** Division **
 * 
 * */
int division(UINT_X *q, UINT_X *reste, UINT_X a, UINT_X b)
{
	/* Test division par zero */
	int t=0;
	for(int i=0; i<b.taille; i++) 
		if(b.tab[i] != 0) t++;
	if(t==0) return fprintf(stderr, "Division par zero impossible\n"), -1;
	/* Autres testes */
	if(q->tab == NULL || q->taille < (a.taille + b.taille+1))
		return fprintf(stderr, "variable quotient non initialisee ou la taille est inférieure à a.taille + b.taille +1\n"), -1;
	if(reste->tab == NULL || reste->taille < b.taille)
		return fprintf(stderr, "variable reste non initialisee ou la taille est inférieure à b.taille \n"), -1;
	/* Test si a < b */
	if(inferieur(a,b)){
		init_uint_x(q);
		for(int i=0; i<a.taille; i++)
			reste->tab[i] = a.tab[i];
		return 0;
	}
	/* Test si a = b */
	if(egal(a,b)){
		init_uint_x(reste);
		init_uint_x(q);
		q->tab[0] = 1;
		return 0;
	}
	
	/* Initialisation */
	UINT_X n = malloc_uint_x(b.taille*64);
	UINT_X p = malloc_uint_x(b.taille*2*64);
	for(int i=0; i<b.taille;i++) 
		p.tab[i] = b.tab[i];
	UINT_X aux = malloc(p.taille*64);
	UINT_X plus = malloc_uint_x(64);
	plus.tab[0] = 1;
	
	
	while(inferieur(p,a) || egal(p,a))
	{
		shift(&p,p,0,1);
		somme(&n,n,plus);
	}
	
	
	free_uint_x(p);
	free_uint_x(aux);
	
	return 0;
}
