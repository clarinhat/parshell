#include "proclist.h"

void flush(void)
{
   if(fflush(fp) != 0) {
       fprintf(stderr, "Error in pthread_mutex_lock()\n");
       exit(EXIT_FAILURE);
   }
}

spList* create_pp() /* Criar a lista */
{
	spList *arr;
	arr = (spList*) malloc(sizeof(spList));
	arr->first = NULL;
	return arr;
}

void destroy_pp(spList *list) /* Apagar a lista/o conteudo e libertar a memoria */
{
	struct saveProcess *curr, *next;
	curr = list->first;
	while(curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	free(list);
}

void newInList(spList *pp, int pid, int stime) 
{
	saveProcessL *sp;
	sp = (saveProcessL *) malloc (sizeof(saveProcessL));
	sp->pid = pid; /* Gravar o pid do processo na lista pp */
	sp->stime = stime; /* Gravar o exit status do processo na lista pp */
	sp->rtime = 0; /* Gravar o tempo do processo na lista pp como 0 pois nao foi calculado ainda*/
	sp->wes = 0; /* Gravar o estado de saida do processo como 0 pois nao foi calculado ainda */
	sp->next = pp->first;
	pp->first = sp;
}

void saveToList(spList *pp, int wp, int wes, int etime, int i) 
{
	saveProcessL *sp;
	sp = pp->first;

	while(sp != NULL) { /* Encontrar o pid na lista */
		fp = fopen("log.txt", "a");
		if(sp->pid == wp) {
			int rtime = etime - sp->stime; /* Calcular o tempo que o processo esta a correr, tempo final - tempo inicial */
			sp->rtime = rtime; /* Gravar runtime do processo */
			sp->wes = wes; /* Gravar o estado de saida do processo */
			tim += sp->rtime;
			fprintf(fp, "iteracao: %d\npid: %d execution time: %d \ntotal execution time: %d\n", i, sp->pid, sp->rtime, tim);
			flush();
			return;
		}
		sp = sp->next;
	}
	printf("O pid %d nao existe na lista \n", wp);
}

void printList(spList *pp) 
{
	saveProcessL *curr;
	curr = pp->first;
	while(curr != NULL) { /* Percorre a lista e imprime todos os processos gravados */
		printf("Pid: %d, Exit Status: %d, Run Time: %d\n", curr->pid, curr->wes, curr->rtime);
		curr = curr->next;
	}
}
