/**
* Arquivo responsável por gerenciamento das operações de sessão crítica
* de read_write da lista encadeada
*
* Autor: Juarez Arce Franco Junior
*/


/**
* Função responsável por gerenciar threads
*/
void controller_threads_read_write(Contexto *context){
	long i=0;
	pthread_t threads[context->qtd_threads];//threads

	//inicializa read-write do contexto
	pthread_rwlock_init(&(context->rwlock),NULL);
	//Cria threads
	for( i=0 ; i <context->qtd_threads ; i++){
		if(pthread_create(&threads[i],NULL,slave_read_write, context)!=0)
			perror(FALHA_CRIAR_THREADS);
	}
	show_loading(context);
	
	//espera todas as threads terminarem suas operações
	for( i=0 ; i < context->qtd_threads ; i++){
		if(pthread_join(threads[i],NULL)!=0)
			perror(FALHA_ESPERAR_THREADS);
	}
	//finaliza read-write do contexto
	pthread_rwlock_destroy(&(context->rwlock),NULL);
}
/**
* Escopo das threads
*/
void* slave_read_write(void* args){
	Contexto *context = args; //contexto do thread princial
	long valor;//valor chave passado por parametro para funções de inserção, remoção e busca
	int operacao;//determina operação que thread deve fazer, inserção, remoção ou busca
	int result;//resultado das funções de inserção e remoção, se foi inserido bem sucedido ou removido com sucesso
	long max=context->qtd_operacoes*5; //determina numero maximo que pode ser gerado para a chave
	Node *node_antecessor;//Variavel utilizada para receber retorno de função buscar antecessor
	int cont_operacoes=0;//contador de operações ja realizadas

	//loop de operações que a thread deve fazer na lista encadeada, faz apenas qtd de operações que usuario solicitou
	while( cont_operacoes < context->qtd_operacoes ){
		operacao = get_randomic_operacao();//verifica qual operação thread deve fazer, inserir, deletar ou buscar.
		valor = 1+rand()%max;//recupera um valor randomico
		//verifica operação de inserção
		if(operacao==INSERT){
			//bloqueia para leitura
			pthread_rwlock_rdlock(&(context->rwlock));
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja inserir
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia
			//bloqueia para inserção e escrita nas variaveis do contexto
			pthread_rwlock_wrlock(&(context->rwlock));
			if( context->cont_operacao_insert < context->qtd_insert && cont_operacoes < context->qtd_operacoes){				
				result = inserir(node_antecessor, valor);//passa referencia do nó anterior para a função inserir. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				context->cont_operacao_insert++;//incrementa contador do contexto de inserções ja feita pelas threads.
				context->cont_insert+=result; //conta inserção bem sucedida, quando elemento não é repetido
			}
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia
			
			
		}
		//verifica operação de remoção
		if(operacao==DELETE ){
			//bloqueia para leitura
			pthread_rwlock_rdlock(&(context->rwlock));
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja remover
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia
			//bloqueia para remoção e escrita nas variaveis do contexto
			pthread_rwlock_wrlock(&(context->rwlock));
			if( context->cont_operacao_delete < context->qtd_delete && cont_operacoes < context->qtd_operacoes){
				result = remover(node_antecessor, valor);//passa referencia do nó anterior para a função remover. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				context->cont_delete+=result;//conta remoção bem sucedida, quando elemento é encontrado na lista
				context->cont_operacao_delete++;//incrementa contador de operação de remoção ja feita
			}
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia			
		}
		//verifica operação de busca
		if(operacao==SEARCH ){
			//bloqueia para leitura
			pthread_rwlock_rdlock(&(context->rwlock));
				node_antecessor = buscarAntecessor(context->lista, valor);
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia
			//bloqueia para escrita nas variaveis do contexto
			pthread_rwlock_wrlock(&(context->rwlock));
			if ( context->cont_operacao_search < context->qtd_search && cont_operacoes < context->qtd_operacoes){
				context->cont_operacao_search++;//incrementa contador do contexto de buscas ja feita
			}
			pthread_rwlock_unlock(&(context->rwlock));//desbloqueia
			
			
		}
		//conta qtd de operações ja realizadas.
		pthread_rwlock_rdlock(&(context->rwlock));//bloqueiado para leitura
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;			
		pthread_rwlock_unlock(&(context->rwlock));//desbloqueia leitura
		
	}
}
