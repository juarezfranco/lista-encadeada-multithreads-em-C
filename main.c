/**
* Universidade Federal da Grande Dourados (UFGD)
* Curso Sistemas de Informação - FACET
*
* Matéria Tópicos em Programação Paralela
* Prof.  M.Sc.  Rodrigo Porfírio da Silva Sacchi
*
* Aluno: Juarez Arce Franco Junior
* Trabalho 1.
*/

// compilar:
// gcc -g -Wall -o <nome_exec> <nome_fonte.c> - lpthread

//Inclui todas as bibliotecas do projeto
#include "libs.h"


/**** VARIÁVEIS ****/
Node *lista;
int qtd_threads;
/** FIM VARIAVEIS **/

int main (int argc, char** argv){
	char opcao;
	//system("clear");
	srand((unsigned)time(NULL));
	do{
		//cria lista
		lista = new_node();
		system("clear");
		printf(CABECALHO);
		printf("Seleciona uma opção:\n");
		printf("(1) Read-write locks.\n");
		printf("(2) Um mutex para a lista inteira\n");
		printf("(3) Um mutex por nó\n");
		printf("(4) Gerenciamento Manual.\n");
		printf("(0) ++ Sair ++\n");
		scanf("%s",&opcao);
		fflush(stdin);
		qtd_threads = 8;
		/** Chamada de funções de CONTROLLERS**/
		switch (opcao){
			case '1':
				start(lista, qtd_threads, READ_WRITE);
				break;
			case '2':
				start(lista, qtd_threads, MUTEX_ALL);
				break;
			case '3':
				start(lista, qtd_threads, MUTEX_BY_NODE);
				break;
			case '4':
				manual(lista);
				break;
			case '0':
				break;
			default:
				printf("Opcão inválida!\n");	
				printf(PRESS_ENTER);	
		}
		//libera lista da memória
		finalizar_lista(lista);
		//Espera pressionar alguma tecla
		if(opcao!='0'){
			getchar();
			getchar();
		}
	}while(opcao!='0');
	printf(RODAPE);
	return 0;
}

int get_randomic_operacao(){
	int opcao;
	opcao = 1+rand()%3;
	//1 = INSERT;
	//2 = DELETE;
	//3 = SEARCH;
	return opcao;
}


/**
* Função responsável por criar threads e escolher qual modo de sessão critica usar
*/
void start(Node* lista, int qtd_threads, int MODO){
	/** Variaveis locais **/
	Contexto *context = new_contexto();//cria contexto para essa função
	unsigned long percent_search, percent_insert, percent_delete;//inputs do usuario
	unsigned long qtd_operacoes,qtd_elementos_iniciais;//inputs do usuario
	unsigned long total_operacoes=0; //recebera soma da qtd de operações realizadas pelas threads
	unsigned long tempo_total=0;//recerá tempo total da execução da tarefa das threads
	char opcao;//inputs usuario
	
	/** Fim variaveis locais **/

	system("clear");
	//define cabeçalho
	if(MODO==READ_WRITE) 	printf(TITLE_READ_WRITE);
	if(MODO==MUTEX_ALL)		printf(TITLE_MUTEX_ALL);
	if(MODO==MUTEX_BY_NODE) printf(TITLE_MUTEX_BY_NODE);

	//Entradas do usuario
	printf("Quantas inserções iniciais devem ser feito na lista?\n: ");
	scanf("%ld",&qtd_elementos_iniciais);
	inicializar_lista(lista, qtd_elementos_iniciais);
	imprimir(lista);
	printf("\nQuantidade de operações que serão realizadas pelas threads?\n: ");
	scanf("%ld",&qtd_operacoes);
	printf("\nPorcentagem de buscas? \n: %%");
	scanf("%ld",&percent_search);
	printf("\nPorcentagem de inserções? \n: %%");
	scanf("%ld",&percent_insert);
	percent_delete = 100-(percent_insert+percent_search);
	printf("\nPorcentagem de remoções: \n: %%%ld\n\n",percent_delete);
	//salva contexto desta função principal
	context->lista 			= lista;
	context->qtd_threads    = qtd_threads;
	context->qtd_operacoes  = qtd_operacoes;
	context->qtd_search		= (qtd_operacoes*percent_search)/100;
	context->qtd_insert 	= (qtd_operacoes*percent_insert)/100;
	context->qtd_delete 	= (qtd_operacoes*percent_delete)/100;

	//captura tempo inicial
	gettimeofday(&(context->t_inicial), NULL);

	//cria threads de acordo com modo
	if(MODO==MUTEX_ALL)
		controller_threads_mutex_all(context);
	if(MODO==MUTEX_BY_NODE)
		controller_threads_mutex_by_node(context);
	if(MODO==READ_WRITE)
		controller_threads_read_write(context);
	
	//captura tempo final
	gettimeofday(&(context->t_final), NULL);

	tempo_total = (context->t_final.tv_sec - context->t_inicial.tv_sec) * 1000000 + 
					((int)context->t_final.tv_usec - (int)context->t_inicial.tv_usec);

	//soma todas as operações
	total_operacoes= context->cont_operacao_insert+context->cont_operacao_delete+context->cont_operacao_search;
	printf("\n._____________RESULTADOS_______________.\n");
	printf("\n Total de operação de inserão......%ld", context->cont_operacao_insert);
	printf("\n Inserções repetidas...............%ld", context->cont_operacao_insert - context->cont_insert);
	printf("\n Total de operação de remoção......%ld", context->cont_operacao_delete);
	printf("\n Remoções bem sucedidas............%ld", context->cont_delete);
	printf("\n Total de operação de buscas.......%ld", context->cont_operacao_search);
	printf("\n");
	printf("\n Total de elementos na lista.......%ld", count(lista));
	printf("\n Total de operações realizadas.....%ld", total_operacoes);
	printf("\n.______________________________________.");
	printf("\nTempo  decorrido:..................%ld  microsegundos\n", tempo_total);
	printf("\n\nDeseja imprimir Lista?\n(s/n): \n");
	scanf("%s",&opcao);
	fflush(stdin);
	if(tolower(opcao)=='s')
		imprimir(lista);

	free(context);//libera da memória contexto dessa função
}






