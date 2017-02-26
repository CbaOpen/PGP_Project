

#ifndef __PRINTF_H
#define __PRINTF_H

struct uint_x_b10{
	int taille;
	char *tab;
	};
typedef struct uint_x_b10 UINT_X_B10;

void printf_binaire_uint_x(UINT_X n);
void printf_b10_uint_x (UINT_X n);

#endif
