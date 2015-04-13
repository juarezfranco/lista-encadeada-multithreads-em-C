/**
* Arquivo responsável por gerenciamento das operações de sessão crítica
* de read_write da lista encadeada
*
* Autor: Juarez Arce Franco Junior
*/


/**
* Função responsável por gerenciar threads
*/
void manager_threads_read_write(Contexto *context, long qtd_threads){
	long i=0;
	pthread_t threads[qtd_threads];//threads
	//Cria threads
	for(i=0;i<qtd_threads;i++){
		if(pthread_create(&threads[i],NULL,slave_read_write, context)!=0)
			perror(FALHA_CRIAR_THREADS);
	}
	//espera todas as threads terminarem suas operações
	for(i=0;i<qtd_threads;i++){
		if(pthread_join(threads[i],NULL)!=0)
			perror(FALHA_ESPERAR_THREADS);
	}
}
/**
* Escopo das threads
*/
void* slave_read_write(void* args){

}
