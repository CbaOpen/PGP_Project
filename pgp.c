#include <stdio.h>
#include <stdlib.h>
#include "pgp.h"
#include "envoi.h"
#include "reception.h"
#include "commandes.h"
#include "lire_ecrire.h"
#include "types.h"

int main(int argc,char** argv){
	int mode=teste_commande_general(argc,argv);
	if(mode==MODE_SIGN_NN_CHIFFRE){
		ecrit_message_non_chiffre(argc,argv);
	}
	return 0;
}
