/**
* Arquivo responsável por conter estrutura do contexto
* de uma função principal da aplicação
*
* Autor: Juarez Arce Franco Junior
*/

//Estrutura com informações do escopo de uma função principal
typedef struct{
	long
		indice,
		qtd_operacoes,
		qtd_search,cont_search, 
		qtd_insert,cont_insert,
		qtd_delete,cont_delete,
		total_operacoes;
	Node *lista;
	pthread_mutex_t list_mutex;
	struct timeval t_inicial, t_final;
}Contexto;

//construtor de contextos;
Contexto *new_contexto(){
	Contexto *context = (Contexto *)malloc(sizeof(Contexto));
	context->indice = 0;
	context->qtd_operacoes=0;
	context->qtd_search=0;
	context->qtd_insert=0;
	context->qtd_delete=0;
	context->total_operacoes=0;
	context->cont_search=0;
	context->cont_insert=0;
	context->cont_delete=0;

	return context;
}