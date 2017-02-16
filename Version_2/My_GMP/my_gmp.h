//definition des structures

#ifndef __MY_GMP
#define __MY_GMP

#define NB_CAR_UINT64 20
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

//type uint_x
//taille est le nombre de cases de type uint64
//tab est un tableau
struct uint_x{
	int taille;
	uint64_t* tab;
}; typedef struct uint_x UINT_X;

//structure pour la somme (et à voir aussi pour la multiplication)
struct resultSomme64{
	uint64_t resultat;
	char finalRetenue;
	}; typedef struct resultSomme64 RESULTSOMME64;
	
#endif
