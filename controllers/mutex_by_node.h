/**
* Arquivo responsável por gerenciamento das operações de sessão crítica
* de um mutex por nó da lista encadeada
*
* Autor: Juarez Arce Franco Junior
*/


/**
* Função responsável por gerenciar threads
*/
void controller_threads_mutex_by_node(Contexto *context){
	long i;
	pthread_t threads[context->qtd_threads];//threads

	//inicializa mutex do contexto
	pthread_mutex_init(&(context->list_mutex),NULL);

	//cria threads
	for(i=0 ; i<context->qtd_threads ; i++){
			if(pthread_create(&threads[i],NULL,slave_mutex_by_node, context)!=0)
				perror(FALHA_CRIAR_THREADS);
	}
	show_loading(context);
	//espera todas as threads terminarem suas operações
	for(i=0 ; i < context->qtd_threads ; i++){
		if(pthread_join(threads[i],NULL)!=0)
			perror(FALHA_ESPERAR_THREADS);
	}
	//finaliza mutex do contexto
	pthread_mutex_destroy(&(context->list_mutex),NULL);
}

/**
* Função retorna ponteiro do elemento antecessor 
* A Função deve buscar até encontrar um nó cujo dado é maior que o valor a ser inserido,
* ou caso chegue ao final da lista, e retornar o elemento que será o antecessor do novo 
* que será inserido.
*/
Node* buscarAntecessorComMutex(Node* raiz,int valor){
	//percorre todos os nós até chegar ao último
	Node* tmp = raiz;
	//verifica se proximo nó é nulo
	pthread_mutex_lock(&(tmp->mutex));// bloqueia nó do proximo loop
	while(tmp->proximo!=NULL){
		//sair do laço se proximo nó tem valor maior que o nó corrente
		if(tmp->proximo->valor >= valor)
			return tmp;//se acontecer o retorno aqui, o nó deve ser desbloquiado na função que receberá nó bloquiado
		pthread_mutex_unlock(&(tmp->mutex));// desbloqueia nó crítica
		
		tmp = tmp->proximo;
		pthread_mutex_lock(&(tmp->mutex));// bloqueia nó do proximo loop
		
	}
	return tmp;//se acontecer o retorno aqui, o nó deve ser desbloquiado na função que receberá nó bloquiado
}

/**
* Escopo das threads
*/
void* slave_mutex_by_node(void* args){
	Contexto *context = args; //contexto do thread princial
	long valor;//valor chave passado por parametro para funções de inserção, remoção e busca
	int operacao;//determina operação que thread deve fazer, inserção, remoção ou busca
	int result;//resultado das funções de inserção e remoção, se foi inserido bem sucedido ou removido com sucesso
	long max=context->qtd_operacoes*10; //determina numero maximo que pode ser gerado para a chave
	Node *node_antecessor;//Variavel utilizada para receber retorno de função buscar antecessor
	int cont_operacoes=0;//contador de operações ja realizadas

	//loop de operações que a thread deve fazer na lista encadeada, faz apenas qtd de operações que usuario solicitou
	while( cont_operacoes < context->qtd_operacoes ){
		operacao = get_randomic_operacao();//verifica qual operação thread deve fazer, inserir, deletar ou buscar.
		valor = 1+rand()%max;//recupera um valor randomico
		
		//conta qtd de operações ja realizadas.
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;			
		//verifica operação de inserção
		if(operacao==INSERT){
			node_antecessor = buscarAntecessorComMutex(context->lista,valor);//recupera nó antecessor do que deseja inserir
			if( context->cont_operacao_insert < context->qtd_insert && cont_operacoes < context->qtd_operacoes){				
				result = inserir(node_antecessor, valor);//passa referencia do nó anterior para a função inserir. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				//Sessão critica para escrita em variaveis do contexto
				pthread_mutex_lock(&(context->list_mutex));//sessão crítica
				context->cont_operacao_insert++;//incrementa contador do contexto de inserções ja feita pelas threads.
				context->cont_insert+=result; //conta inserção bem sucedida, quando elemento não é repetido
				pthread_mutex_unlock(&(context->list_mutex));//sessão crítica	
			}
			pthread_mutex_unlock(&(node_antecessor->mutex));// desbloqueia nó da sessão crítica
		}
		//conta qtd de operações ja realizadas.
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;			
		//verifica operação de remoção
		if(operacao==DELETE ){
			node_antecessor = buscarAntecessorComMutex(context->lista,valor);//recupera nó antecessor do que deseja remover
			if( context->cont_operacao_delete < context->qtd_delete && cont_operacoes < context->qtd_operacoes){
				result = remover(node_antecessor, valor);//passa referencia do nó anterior para a função remover. Assim elimina a necessidade de fazer busca dentro do seu escopo				
				//Sessão critica para escrita em variaveis do contexto
				pthread_mutex_lock(&(context->list_mutex));//sessão crítica
				context->cont_delete+=result;//conta remoção bem sucedida, quando elemento é encontrado na lista
				context->cont_operacao_delete++;//incrementa contador de operação de remoção ja feita
				pthread_mutex_unlock(&(context->list_mutex));//sessão crítica
			}
			pthread_mutex_unlock(&(node_antecessor->mutex));//sessão crítica
			
		}
		//conta qtd de operações ja realizadas.
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;			
		//verifica operação de busca
		if(operacao==SEARCH ){
			node_antecessor = buscarAntecessorComMutex(context->lista, valor);
			if ( context->cont_operacao_search < context->qtd_search && cont_operacoes < context->qtd_operacoes){
				//Sessão critica para escrita em variaveis do contexto
				pthread_mutex_lock(&(context->list_mutex));//sessão crítica
				context->cont_operacao_search++;//incrementa contador do contexto de buscas ja feita
				pthread_mutex_unlock(&(context->list_mutex));//sessão crítica	
			}
			pthread_mutex_unlock(&(node_antecessor->mutex));// desbloqueia sessão crítica desse nó
		}
		//conta qtd de operações ja realizadas.
		cont_operacoes = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;			
		
	}

}

