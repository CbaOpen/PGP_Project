#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "genere_cle.h"
//#include "nb_premier.h"

//........................GENERATION CLE RSA..........................//


//Crible d'Eratostène
//fait la liste des nombres premier jusqu'à un nombre n et les met dans un fichier
void liste_nb_premier(int n, FILE* file)
{
	int nb=2, i, test=1, cmp=0;
	
	while(nb<n){
		for(i=2; i<sqrt(n); i++)
			{ if(nb%i == 0) test = 0; }
		if(test) {fprintf(file,"%d ",nb); cmp++;}
		nb++;
		test = 1;
		}
	printf("%d\n",cmp);
	
}

//n = 2^16 est un nombre suffisant pour que la multiplication p*q soir inférieur à 32 bits
//si p*q supérieur à 32 bits, overflow lors du calcul de l'inverse
void crée_liste_nb_premier(){
	
	FILE * file = fopen("Liste_nb_premier","w");
	if (file == NULL) printf("impossible d'ouvrir le fichier");
	
	liste_nb_premier(pow(2,16), file);
	
	fclose(file);
	}


//choisi un nombre aléatoire parmi les nombres premiers du fichier entre le 5000ème et le dernier
//6488 est le nombre total de nombres premiers dans le fichier
//ce chiffre change si la valeur max n de la fonction qui calcul les nombres premiers change
int alea()
{ 
	int i=rand()%(6488-5000) + 5000; printf("%d\n",i);
	int cmp=1, n;
	FILE* file = fopen("Liste_nb_premier","r");
	if(file == NULL) { printf("le fichier liste_nb_premier n'existe pas"); exit(EXIT_FAILURE); }
	
	while(cmp<i) { fscanf(file,"%d ",&n); cmp++; }
	fscanf(file,"%d ",&n);
	return n; 
}


uint64_t pgcd(uint64_t a,uint64_t b)
{
	if (a % b == 0) return b;
	return pgcd(b, a % b);
}

uint64_t genere_e(uint64_t z){
	int e=(rand()*rand()*rand()*rand())%z; //rand puissance 4 pour avoir un nombre suffisemment grand
	if(pgcd(e,z)==1) return e;
	return genere_e(z);
}

//basé sur l'algorithme d'Euclide étendu
uint64_t genere_d(uint64_t e, uint64_t z){
	int64_t z0,t,q,r0,r1;
	
	z0 = z;
	r0=0; 
	r1=1;
	
	while(e>1){
		q= e/z;
		t=z;
		z = e%z;
		e=t;
		t=r0;
		r0=r1 - q*r0;
		r1=t;
		}
	while(r1<0){ r1 += z0;}
	return r1;
	
}

void genere_cle(uint64_t *p, uint64_t *q, uint64_t *n, uint64_t *e, uint64_t *d)
{
	srand(time(NULL));
	*p=alea();
	*q=alea();
	
	printf("\np=%llu\n",(long long)*p);
	printf("q=%llu\n",(long long)*q);
	
	*n = (*p)*(*q);
	printf("n=%lld\n",(long long)*n);
	
	uint64_t z = (*p-1)*(*q-1);
	printf("z=%lld\n",(long long)z);
	
	*e = genere_e(z);
	printf("e=%lld\n",(long long)*e);
	
	*d = genere_d(*e,z);
	printf("d=%lld\n",(long long)*d);
	printf("\n");
}

//.........................GENERATION CLE SESSION.....................//

//A faire :p
