/**
* Arquivo responsável por gerenciamento das operações de sessão crítica
* de um mutex para a lista inteira
*
* Autor: Juarez Arce Franco Junior
*/

/**
* Função gerencia Threads, cria e finaliza
*/
void controller_threads_mutex_all(Contexto *context){
	long i;
	pthread_t threads[context->qtd_threads];//threads

	//inicializa mutex do contexto
	pthread_mutex_init(&(context->list_mutex),NULL);

	//cria threads
	for(i=0;i < context->qtd_threads ; i++){
		if(pthread_create(&threads[i],NULL, slave_mutex_all , context)!=0)
			perror(FALHA_CRIAR_THREADS);
	}

	show_loading(context);

	//espera todas as threads terminarem suas operações
	for(i=0;i < context->qtd_threads;i++){
		if(pthread_join(threads[i],NULL)!=0)
			perror(FALHA_ESPERAR_THREADS);
	}
	//finaliza mutex do contexto
	pthread_mutex_destroy(&(context->list_mutex),NULL);


}

/**
* Escopo das threads
*/
void* slave_mutex_all(void *args){
	Contexto *context = args; //contexto do thread princial
	long valor;//valor chave passado por parametro para funções de inserção, remoção e busca
	int operacao;//determina operação que thread deve fazer, inserção, remoção ou busca
	int result;//resultado das funções de inserção e remoção, se foi inserido bem sucedido ou removido com sucesso
	long max=context->qtd_operacoes*10; //determina numero maximo que pode ser gerado para a chave
	Node *node_antecessor;//Variavel utilizada para receber retorno de função buscar antecessor
	long cont_operacoes=0;//contador de operações ja realizadas

	//loop de operações que a thread deve fazer na lista encadeada, faz apenas qtd de operações que usuario solicitou
	while( cont_operacoes < context->qtd_operacoes ){
		operacao = get_randomic_operacao();//verifica qual operação thread deve fazer, inserir, deletar ou buscar.
		valor = 1+rand()%max;//recupera um valor randomico
		//verifica operação de inserção
			
		if(operacao==INSERT){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica
			if ( context->cont_operacao_insert < context->qtd_insert ){
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja inserir
				result = inserir(node_antecessor, valor);//passa referencia do nó anterior para a função inserir. Assim elimina a necessidade de fazer busca dentro do seu escopo														
				context->cont_insert+=result;//conta inserção bem sucedida, quando não inserção é repetida
				context->cont_operacao_insert++;//incrementa contador do contexto de inserções ja feita pelas threads.
			}
			pthread_mutex_unlock(&(context->list_mutex));//libera da sessão crítica	
		}
		//verifica operação de remoção
		if(operacao==DELETE){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica
			if( context->cont_operacao_delete < context->qtd_delete ){			
				node_antecessor = buscarAntecessor(context->lista,valor);//recupera nó antecessor do que deseja inserir
				result = remover(node_antecessor, valor);//passa referencia do nó anterior para a função remover. Assim elimina a necessidade de fazer busca dentro do seu escopo							
				context->cont_delete+=result;//conta remoção bem sucedida, quando elemento é encontrado na lista
				context->cont_operacao_delete++;//incrementa contador de remoção ja feita
			}
			pthread_mutex_unlock(&(context->list_mutex));//libera da sessão crítica
		}
		//verifica operação de busca
		if(operacao==SEARCH){
			pthread_mutex_lock(&(context->list_mutex));//sessão crítica
			if( context->cont_operacao_search < context->qtd_search ){
					node_antecessor = buscarAntecessor(context->lista, valor);			
					context->cont_operacao_search++;//incrementa contador do contexto de buscas ja feita
			}
			pthread_mutex_unlock(&(context->list_mutex));//libera da sessão crítica	
		}
		//conta qtd de operações ja realizadas.
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;	
	}

}

