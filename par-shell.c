#include "par-shell.h"

void mutex_lock(void) 
{
	if(pthread_mutex_lock(&lock) != 0) { /* tenta fechar o trinco, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in pthread_mutex_lock()\n");
		exit(EXIT_FAILURE);
	}
}

void mutex_unlock(void) 
{
	if(pthread_mutex_unlock(&lock) != 0) { /* tenta abrir o trinco, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in pthread_mutex_unlock()\n");
		exit(EXIT_FAILURE);
	}
}

void mutex_init(void)
{
	if(pthread_mutex_init(&lock, NULL) != 0) { /* tenta iniciar o trinco, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_mutex_init(lock)\n");
    	exit(EXIT_FAILURE);
  	}
}

void mutex_destroy(void)
{
	if((pthread_mutex_destroy(&lock)) != 0) { /* tenta destruir o trinco, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_mutex_destroy(lock)\n");
    	exit(EXIT_FAILURE);
  	}
}

void cond_wait_t(void) 
{
	if(pthread_cond_wait(&t, &lock) != 0) {	/* tenta fazer wait da variavel de condicao t, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in  pthread_cond_wait(t)\n");
		exit(EXIT_FAILURE);
	}
}

void cond_wait_c(void) 
{
	if(pthread_cond_wait(&c, &lock) != 0) { /* tenta fazer wait da variavel de condicao c, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in pthread_cond_wait(c)\n");
		exit(EXIT_FAILURE);
	}
}

void cond_signal_t(void) 
{
	if(pthread_cond_signal(&t) != 0) { /* tenta fazer signal da variavel de condicao t, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in  pthread_cond_signal()\n");
		exit(EXIT_FAILURE);
	}
}

void cond_signal_c(void) 
{
	if(pthread_cond_signal(&c) != 0) { /* tenta fazer signal da variavel de condicao c, se nao consegue imprime o erro e sai */
		fprintf(stderr, "Error in pthread_cond_signal()\n");
		exit(EXIT_FAILURE);
	}
}

void cond_init_c()
{
	if(pthread_cond_init(&c, NULL) != 0) { /* tenta iniciar a variavel de condicao c, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_cond_init(c)\n");
    	exit(EXIT_FAILURE);
  	}
}

void cond_init_t()
{
	if(pthread_cond_init(&t, NULL) != 0) { /* tenta iniciar a variavel de condicao t, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_cond_init(t)\n");
    	exit(EXIT_FAILURE);
  	}
}

void cond_destroy_c()
{
	if(pthread_cond_destroy(&c) != 0) { /* tenta destruir a variavel de condicao c, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_cond_destroy(c)\n");
    	exit(EXIT_FAILURE);
  	}
}

void cond_destroy_t()
{
	if(pthread_cond_destroy(&t) != 0) { /* tenta destruir a variavel de condicao t, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_cond_destroy(t)\n");
    	exit(EXIT_FAILURE);
  	}
}

void thread_join(pthread_t thread)
{
	if(pthread_join(thread, NULL) != 0) { /* a tarefa principal tenta esperar pela monitora, se nao consegue imprime o erro e sai */
    	fprintf(stderr, "Error in pthread_join(thread)\n");
    	exit(EXIT_FAILURE);
  	}
}

void *timer()
{
	int wp, status;
	
	while(1) {
		mutex_lock();
		while(numSon == 0 && ex == 0) {	/* enquanto nao ha processos filhos nem se fez exit a tarefa espera */
			cond_wait_c();
		}
		if(ex == 1 && numSon == 0) {	/* se se fez o comando exit e todos os processos filhos terminaram, faz-se exit da thread */
			mutex_unlock();
			pthread_exit(NULL);
		}
		mutex_unlock();
		if((wp = wait(&status)) != -1) {
			int wes = WEXITSTATUS(status); /* 0 - o processo saiu bem || !0 - o processo saiu mal*/
			int etime = time(NULL); /* O tempo depois de o processo acabar */
			mutex_lock();
			saveToList(pp, wp, wes, etime, iter); /* Guardar os valores para a lista ligada */
			iter++;	/* incremenat o valor da iteracao */
			numSon--;	/* decrementa-se o numero de processos filhos activos*/
			mutex_unlock();	
			cond_signal_t();	/* faz um signal para o processo filho que esteja a espera de poder executar */	
		}
	}
}

int main()
{	
	int tid, stime, pid, argNum;	/* Inicializacao do pid da tarefa, do tempo inicial, do pid e do numero de argumentos */
	char *str[MAXARGS];	/* Argumentos para o readLineArguments ler */
	pp = create_pp(); /* Criar lista ligada para guardar valores */
	pthread_t thread; /* Criar tarefa */

	mutex_init();	/* Inicializar o trinco */
	cond_init_c();	/*	Inicializar a variavel de condicao que ira controlar os processos a serem guardados */
	cond_init_t();	/*	Inicializar a variavel de condicao que ira controlar os processos em execucao */

	tim = 0;
	if((fp = fopen("log.txt", "r")) != NULL) {	/* verifica se o ficheiro ja foi escrito (se tem informacao)*/
		while(fgets(buff, BUFFSIZE, fp) != NULL) {
			sscanf(buff, "iteracao: %d", &iter);	/* retira qual foi a ultima iteracao realizada */
			sscanf(buff, "total execution time: %d", &tim);	/* retira qual foi o ultimo tempo total calculado */
		}
		iter++;	/* incremneta o valor da iteracao */
	}

	tid = pthread_create(&thread, NULL, timer, NULL); /* Cria uma tarefa nova */
	if(tid) {	/* Se nao conseguir criar uma tarefa nova output o erro e sai */
		fprintf(stderr, "Failed to create thread. \n"); 
		exit(EXIT_FAILURE);
	}
	while(1) {
		argNum = readLineArguments(str, MAXARGS, buff, BUFFSIZE); /* Ler input do utilizador */	

		if(argNum == 0 || argNum > MAXARGS) {	/* Caso nao tenha o numero correto de argumentos: Erro */
			fprintf(stderr, "Arguments incorrect.\n");
		}
		else if(strcmp(str[0],"exit") == 0) { /* exit command */
			mutex_lock();
			ex = 1;	/* varivel que avisa que foi feito o comando exit*/
			mutex_unlock();
			cond_signal_c();
			fclose(fp);      /*	fecha o ficheiro */
			thread_join(thread);	/* a tarefa principal vai esperar a monitora faça exit */
			printList(pp); /* Imprime a lista de resultados dos processos */
			cond_destroy_c();	/* Destruicao da variavel de condicao c */
			cond_destroy_t();	/* Destruicao da variavel de condicao t */
			mutex_destroy();	/* Destruicao do trinco */
			destroy_pp(pp); /* Apaga a lista e liberta a memoria */
			exit(EXIT_SUCCESS); /* O programa acabou com sucesso */
		}
		else { /* pathname [arg1 arg2 ...] command */
			stime = time(NULL);
			mutex_lock();
			while(numSon == MAXPAR) cond_wait_t();	/* enquanto o numero de processos filhos a serem executados for o maximo, este espera */
			mutex_unlock();
			pid = fork(); /* Criar novo processo filho */

			if(pid == 0) { /* Processo filho */
				execv(str[0], str);
				fprintf(stderr, "Error while running child process.\n");
				exit(EXIT_FAILURE); /* Exit se execv retornar erro */
			}
			else if(pid > 0) {
				mutex_lock();
				numSon++; /* Incrementar o contador de processos filhos */
				newInList(pp, pid, stime);
				mutex_unlock();
				cond_signal_c();	/* Faz signal de que este processo pode ser guardado na lista*/
			}
			else if(pid < 0) { /* Erro na criacao do processo filho */
				fprintf(stderr, "Error while forking child process.\n");
				exit(EXIT_FAILURE); /* Exit se houver erro */
			}
		}
	}
}