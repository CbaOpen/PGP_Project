#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "genere_cle.h"

//........................GENERATION CLE RSA..........................//

//2 puissance 32 : valeur max pour en entier non signé de 32 bits
//j'ai pris 32 bits pour p et q car quand on multiplie q*p
//cela donne un nombre en dessous de la valeur max pour les nombres de 64 bits
int alea()
{
	uint64_t n=pow(2,32);  
	uint64_t i=rand()%n;
	return i;
}

int teste_premier(uint64_t a)
{
	if(a==2) return 1;
	else if(a%2==0) return 0;
	uint64_t n;
	uint64_t aa=sqrt(a);
	int test=0;
	for(n=1; n<=aa; n=n+2){
		if(a%n==0){
			test++;
		}
	}
	if(test==2) return 1;
	return 0;
}

int alea_premier()
{
	uint64_t i=alea();
	if(teste_premier(i)) return i;
	else {
		return alea_premier();
	}
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
	uint64_t z_0,e_0,t,t_0,q,r,temp;
	
	z_0 = z;
	e_0 = e;
	t_0 = 0;
	t = 1;
	q = z_0/e_0;
	r = z_0 - q*e_0;
	
	while(r>0){
		temp = (t_0-q*t)%z;
		t_0 = t;
		t = temp;
		z_0 = e_0;
		e_0 = r;
		q = z_0/e_0;
		r = z_0 - q*e_0;
		}
	if (e_0 != 1) { printf("e n'a pas d'inverse mod n"); return 0; }
	return t;
	
}

void genere_cle(uint64_t *p, uint64_t *q, uint64_t *n, uint64_t *e, uint64_t *d)
{
	srand(time(NULL));
	*p=alea_premier();
	*q=alea_premier();
	while(*p==*q){
		*p=alea_premier();
		*q=alea_premier();
	}
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
