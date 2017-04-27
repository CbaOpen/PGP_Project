

#ifndef __PRINTF_H
#define __PRINTF_H


/** Type uint_x_b10
 * uint_x_b10 est la représentation en base 10 d'une variable uint_x
 * taille est le nombre de cases de type uint64
 * tab est un pointeur sur un tableau de type uint64 */
struct uint_x_b10{
	int taille;
	char *tab;
	};
typedef struct uint_x_b10 UINT_X_B10;

void printf_binaire_uint_x(UINT_X n);
void printf_b10_uint_x (UINT_X n);

#endif
