#ifndef PARSHELL_H
#define PARSHELL_H

#include "commandlinereader.h" /* readLineArguments */
#include "proclist.h" /* A lista dos processos lista ligada */
#include <unistd.h> /* execv */
#include <string.h> /* strcmp */
#include <sys/wait.h> /* wait */
#include <pthread.h> /* thread creation/manipulation/destruction */
#include <time.h> /* time() */

#define MAXARGS 7	/* Maximo de argumentos a seres lidos pelo creadLineArguments */
#define MAXPAR 4	/* Maximo de processos filho que a par-shell pode ter a correr a cada instante */

int ex = 0;
int iter = 0;
spList *pp;
int numSon = 0;	/* Contador de processos filhos */
pthread_mutex_t lock;
pthread_cond_t c, t;

#endif