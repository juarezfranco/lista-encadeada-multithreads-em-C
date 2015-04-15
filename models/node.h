/**
* Arquivo responsável por conter estrutura e funções 
* do nó da lista encadeada
*
* Autor: Juarez Arce Franco Junior
*/


// Estrutura representa Nó da lista encadeada
typedef struct str_node{
	int valor;//chave
	struct str_node* proximo;//ponteiro para o próximo nó da lista
	pthread_mutex_t mutex;//mutex do nó
}Node;

/**
* Construtor de nó
*/
Node* new_node(void){
	Node* node = (Node *) malloc(sizeof(Node));
	node->proximo= NULL;
	//inicializa mutex do nó
	pthread_mutex_init(&(node->mutex),NULL);
	return node;
}




/**
* Libera da memoria todos os nós da lista
*/
void finalizar_lista(Node* lista){
	//chamada recursiva até chegar ao final da lista
	if(lista->proximo!=NULL)
		finalizar_lista(lista->proximo);
	//destrói mutex do nó
	pthread_mutex_destroy(&(lista->mutex),NULL);
	//libera da memoria nó da lista
	free(lista);
	return;
}



/**
* Função retorna ponteiro do elemento antecessor 
* A Função deve buscar até encontrar um nó cujo dado é maior que o valor a ser inserido,
* ou caso chegue ao final da lista, e retornar o elemento que será o antecessor do novo 
* que será inserido.
*/
Node* buscarAntecessor(Node* raiz,int valor){
	//percorre todos os nós até chegar ao último
	Node* tmp = raiz;
	//verifica se proximo nó é nulo
	while(tmp->proximo!=NULL){
		//sair do laço se proximo nó tem valor maior que o nó corrente
		if(tmp->proximo->valor >= valor)
			return tmp;
		tmp = tmp->proximo;
	}
	return tmp;
}

/**
* Função inserir novo nó na lista encadeada ordenada.
*/
int inserir(Node* antecessor, int valor){
	Node *tmp;
	//cria novo nó que será inserido
	Node* novo_node = new_node();
	novo_node->valor=valor;
	//insere novo nó na lista
	//se proximo nó é nulo então novo nó será adicionado no final da lista
	if(antecessor->proximo==NULL){
		antecessor->proximo = novo_node;
		return 1;
	}
	//verifica se valores são iguais
	if(antecessor->proximo->valor==valor){
		//destrói seu mutex
		pthread_mutex_destroy(&(novo_node->mutex),NULL);
		//desfaz novo nó da memoria
		free(novo_node);
		return 0;
	}
	//Novo nó será adicionado entre dois nós
	tmp = antecessor->proximo;
	//insere novo nó depois depois do antecessor
	antecessor->proximo = novo_node;
	//aponta o proximo nó para o sucessor
	novo_node->proximo = tmp;
	
	return 1;
}

/**
* Função remover nó da lista encadeada
* @param recebe nó anterior e com ele verifica o proximo nó se é o valor que realmente deseja remover
*/
int remover(Node* antecessor, int valor){
	//busca nó para fazer remoção.
	Node *lixo = antecessor->proximo;
	//verifica se lixo realmente existe
	if(lixo==NULL){
		return;
	}
	//verifica se nó tem o mesmo valor, se não elemento nao existe
	if(lixo->valor==valor){
		//atualiza nó do antecessor ligando ao proximo do que sera excluido
		antecessor->proximo = lixo->proximo;
		//destrói mutex do nó
		pthread_mutex_destroy(&(lixo->mutex),NULL);
		free(lixo);//exclui nó da memoria
		return 1;
	}else{
		return 0;
	}
	return 0;
}

/**
* Função imprimi todos os nós da da lista encadeada
*/
void imprimir(Node* raiz){
	//percorre todos os nós até chegar ao último
	Node* tmp = raiz;
	while(tmp->proximo!=NULL){
		//recupera proximo nó
		tmp = tmp->proximo;
		printf("|%d", tmp->valor);
	}
	//exibir barra apenas se lista possuir pelo menos um elemento
	if(raiz->proximo!=NULL)
		printf("|");
	printf("\n");
}

/**
* Função inicializa lista com elementos
*/
void inicializar_lista(Node* lista, int qtd_elementos){
	int valor,i,result=0;
	Node *node_antecessor;
	for(i=0; i<qtd_elementos; i++){
		valor = 1+ (rand()%(qtd_elementos*5));
		node_antecessor = buscarAntecessor(lista,valor);
		result += inserir(node_antecessor, valor);
	}
	printf("Total de elementos inseridos %d\n",result);
	printf("Total de elementos repetidos %d\n", qtd_elementos-result);
}

long count(Node *lista){
	long cont=0;
	Node *tmp = lista;
	while(tmp->proximo!=NULL){
		tmp = tmp->proximo;
		//printf("%ld\n",tmp->valor);
		cont++;
	}
	return cont;
}