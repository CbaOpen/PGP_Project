# Claire Baskevitch - Clément Caumes
# Année 2017
# Projet Pretty Good Privacy (PGP)

# Makefile du projet PGP

# Variables ====================================================================

## Structure du projet ........................................................:

export PROJECT = PGP_Project
EXE_NAME = pgp

export WD_PATH = $(shell pwd | sed 's: :\\ :g')/
export SRC_PATH = src/
export INC_PATH = inc/
OBJ_PATH = obj/
EXE_PATH = exe/
ENV_PATH = env/
REP_PATH = report/
DOC_PATH = doc/

EXEC = $(EXE_PATH)$(EXE_NAME)
export SRC = $(shell find $(SRC_PATH)*.c)
export INC = $(shell find $(INC_PATH)*.h)
OBJ = $(SRC:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

## Compilation ................................................................:

CC = gcc
INC_FLAGS    = -I$(INC_PATH)
#DEP_FLAGS    = -MMD -MP
PROJ_CFLAG   = -c -g3 -Wall
PROJ_LDFLAGS = -Wall -lgmp -lm

CFLAGS  = $(PROJ_CFLAG) $(INC_FLAGS)
LDFLAGS = $(PROJ_LDFLAG)

## Lancement ..................................................................:

ARGS = -h

# Cibles =======================================================================

.PHONY : clean mrproper indent report doc

## Lancement ..................................................................:

run : compil 
	@echo "--> Lancement de '$(EXEC)' :"
	$(EXEC) $(ARGS)

## Compilation ................................................................:

compil : $(EXEC)

$(EXEC) : $(OBJ) 
	@echo "--> Édition des liens dans '$(EXEC)' :"
	$(CC) $^ -o $(EXEC) $(OBJ) $(LDFLAGS)

$(OBJ_PATH)%.o : $(SRC_PATH)%.c Makefile
	@echo "--> Compilation de '$<' :"
	$(CC) -c $< -o $@ $(CFLAGS)

## Dépendances ................................................................:

-include $(OBJ:%.o=%.d)

## Nettoyage ..................................................................:

clean :
	@echo "--> Suppression des fichier temporaires de $(PROJECT) :"
	rm -f $(OBJ_PATH)*.o $(OBJ_PATH)*.d $(SRC_PATH)*~ $(INC_PATH)*~
	find . -name .fuse_hidden* -exec rm -f '{}' \;
	@make clean --directory="$(REP_PATH)" --no-print-directory

mrproper : clean
	@echo "--> Suppression de l'exécutable et des fichiers produits" \
	    "de $(PROJECT) :"
	rm -f $(EXE_PATH)* $(ENV_PATH)*_cypher
	@make clean --directory="$(DOC_PATH)" --no-print-directory
	@echo "--> Nettoyage complet du dossier de travail de $(PROJECT)" \
	    "effectué !"

## Debugger & Profiler ........................................................:

gdb : compil
	@echo "--> Debbugage avec $@ :"
	$@ -q --args $(EXEC) $(ARGS)

valgrind-p1 : compil
	@echo "--> Debbugage avec $@ (profile 1) :"
	valgrind --tool=memcheck --leak-resolution=high \
	    --show-possibly-lost=yes --show-reachable=yes $(EXEC) $(ARGS)

valgrind-p2 : compil
	@echo "--> Debbugage avec $@ (profile 2) :"
	valgrind --tool=memcheck --leak-resolution=high --leak-check=full \
	    --show-possibly-lost=yes --show-reachable=yes --track-origins=yes \
	    $(EXEC) $(ARGS)

## Présentation ...............................................................:

indent :
	@echo "--> Reformatage de la présentation du code (paramètres dans" \
	    ".indent.pro) :"
	$@ $(SRC_PATH)* $(INC_PATH)*

## Rapport & Documentation ....................................................:

report :
	@make --directory="$(REP_PATH)" --no-print-directory 

doc :
	@make --directory="$(DOC_PATH)" --no-print-directory MASTER=1
