#ifndef PROLIST_H
#define PROLIST_H

#include <stdio.h> /* impressao de dados */
#include <stdlib.h>  /* EXIT e manipilacao de memoria */

typedef struct saveProcess { /* Estrutura para guardar informacao do processo */
	int pid; /* Onde guardar o pid do processo */
	int wes; /* Onde guardar o exit status do processo */
  	int stime; /* Onde guardar o tempo de comeco do processo */
	int rtime; /* Onde guardar o tempo total do processo */
  	struct saveProcess *next;
} saveProcessL;

typedef struct {
	saveProcessL* first;
} spList;

FILE *fp;
#define BUFFSIZE 100
char buff[BUFFSIZE];
int tim;

spList* create_pp();

void destroy_pp(spList *array);

void newInList(spList *pp, int pid, int stime);

void saveToList(spList *pp, int wp, int wes, int etime, int iter);

void printList(spList *pp);

#endif