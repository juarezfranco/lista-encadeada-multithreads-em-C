/**
* Arquivo responsável por gerenciamento das operações de sessão crítica
* de um mutex para a lista inteira
*
* Autor: Juarez Arce Franco Junior
*/

/**
* Função gerencia Threads, cria e finaliza
*/
void manager_threads_mutex_all(Contexto *context, long qtd_threads){
	long i;
	qtd_threads=8;
	pthread_t threads[qtd_threads];//threads
	for(i=0;i<qtd_threads;i++){
		if(pthread_create(&threads[i],NULL, slave_mutex_all , context)!=0)
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
void* slave_mutex_all(void *args){
	Contexto *context = args; // recupera contexto da thread principal
	long valor;//valor da chave que será inserido
	int operacao;//variavel define operações inserção, remoção, ou busca
	long max=context->qtd_operacoes*10; //evita multiplicação no laço
	Node *node_antecessor;//Variavel utilizada para receber retorno de função buscar antecessor
	printf("oi\n");
	//incrementa indice para entrar poder entra no laço for
	//se não incrementar aqui, vai ultrapassar a quantidade de inserções ao entrar no laço for
	 context->indice++;

	//loop de operações que a thread deve fazer na lista encadeada, faz apenas qtd de operações que usuario solicitou
	for( ; context->indice <= context->qtd_operacoes ; ){
		operacao = get_randomic_operacao();//verifica qual operação thread deve fazer, inserir, deletar ou buscar.
		valor = 1+rand()%max;//recupera um valor randomico
		//verifica operação de inserção
		
		if(operacao==INSERT && ( context->cont_insert < context->qtd_insert )){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica	
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja inserir
				inserir(node_antecessor, valor);//passa referencia do nó anterior para a função inserir. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				context->cont_insert++;//incrementa contador do contexto de inserções ja feita pelas threads.
				context->total_operacoes++;//incrementa contadaor do contexto do total de operações ja feita pelas threads
				context->indice++;//incrementa indice, condição de saida do for	
			pthread_mutex_unlock(&(context->list_mutex));
		}
		//verifica operação de remoção
		if(operacao==DELETE && ( context->cont_delete < context->qtd_delete )){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja inserir
				remover(node_antecessor, valor);//passa referencia do nó anterior para a função remover. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				context->cont_delete++;//incrementa contador de remoção ja feita
				context->total_operacoes++;
				context->indice++;//incrementa indice, condição de saida do for	
			pthread_mutex_unlock(&(context->list_mutex));
		}
		//verifica operação de busca
		if(operacao==SEARCH && ( context->cont_search < context->qtd_search )){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica
				buscarAntecessor(context->lista, valor);
				context->cont_search++;//incrementa contador do contexto de buscas ja feita
				context->total_operacoes++;//incrementa contador do contexto do total de operações feitas pelas threads
				context->indice++;//incrementa indice, condição de saida do for	
			pthread_mutex_unlock(&(context->list_mutex));
			
		}	
	}
}

