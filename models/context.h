/**
* Arquivo responsável por conter estrutura do contexto
* de uma função principal da aplicação
*
* Autor: Juarez Arce Franco Junior
*/

//Estrutura com informações do escopo de uma função principal
typedef struct{
	int qtd_threads;		 //qtd total de threads deste
	long	 
		qtd_operacoes,		 //qtd total de operações que usuario solicitou
		qtd_search,			 //qtd total de operações de busca que usuario solicitou
		qtd_insert,			 //qtd total de operações de inserção que usuario solicitou
		qtd_delete,			 //qtd total de operações de remoção que usuario solicitou
		cont_operacao_search,//contador de operação de busca ja realizadas
		cont_operacao_insert,//contador de operação de inserções ja realizadas
		cont_operacao_delete,//contador de operação de remoções ja realizadas
		cont_insert,		 //contador de inserções bem sucedidas, sem contar inserções iguais
		cont_delete;		 //contador de remoções bem sucedidas
	Node *lista;
	pthread_mutex_t list_mutex;
	pthread_rwlock_t rwlock;
	struct timeval t_inicial, t_final;
}Contexto;

//construtor de contextos;
Contexto *new_contexto(){
	Contexto *context = (Contexto *)malloc(sizeof(Contexto));
	context->qtd_threads=0;
	context->qtd_operacoes=0;
	context->qtd_search=0;
	context->qtd_insert=0;
	context->qtd_delete=0;
	context->cont_insert=0;
	context->cont_delete=0;
	context->cont_operacao_search=0;
	context->cont_operacao_insert=0;
	context->cont_operacao_delete=0;
	//inicializa mutex do contexto
	return context;
}
//Libera da memória o contexto
void finaliza_context(Contexto *context){
	//finaliza mutex do contexto
	free(context);
}

/**
* Função responsável por resetar contadores do contexto.
* Assim o contexto pode ser utilizado em outra operação
* com apenas as entradas do usuarios.
*/
void resetar_contexto(Contexto *context){
	context->cont_operacao_search=0;
	context->cont_operacao_insert=0;
	context->cont_operacao_delete=0;
	context->cont_insert=0;
	context->cont_delete=0;
	//context->t_inicial.tv_sec=0;
	//context->t_inicial.tv_usec=0;
	//context->t_final.tv_sec=0;
	//context->t_final.tv_usec=0;
	
}