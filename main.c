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
// gcc -g -Wall -o <nome_exec> <nome_fonte.c> -lpthread -lm

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
		__fpurge(stdin);
		qtd_threads = 16;
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

/**
* Função responsável por criar threads e escolher qual modo de sessão critica usar
*/
void start(Node* lista, int qtd_threads, int MODO){
	/** Variaveis locais **/
	Contexto *context = new_contexto();//cria contexto para essa função
	int percent_search, percent_insert, percent_delete;//inputs do usuario
	int qtd_operacoes,qtd_elementos_iniciais;//inputs do usuario
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
	//input_int("Quantas inserções iniciais devem ser feito na lista?\n: ",&qtd_elementos_iniciais);
	//inicializar_lista(lista, qtd_elementos_iniciais);
	//imprimir(lista);
	input_int("\nQuantidade de operações que serão realizadas pelas threads?\n: ", &qtd_operacoes);
	input_int("\nPorcentagem de buscas? \n: %",&percent_search);
	input_int("\nPorcentagem de inserções? \n: %",&percent_insert);
	percent_delete = 100-(percent_insert+percent_search);
	printf("\nPorcentagem de remoções: \n: %%%d\n\n",percent_delete);
	//salva contexto desta função principal
	context->lista 			= lista;
	context->qtd_threads    = qtd_threads;
	context->qtd_search		= round((qtd_operacoes*percent_search)/100.0);//round arredonda pra inteiro mais proximo, se não usar isso 
	context->qtd_insert 	= round((qtd_operacoes*percent_insert)/100.0);//o loop pode ser infinito quando threads forem executar as operações.
	context->qtd_delete 	= round((qtd_operacoes*percent_delete)/100.0);//Para compilar precisa do parametro -lm
	//ajusta qtd para ficar exata com a total de operacoes soliticadas pelo usuario,
	//pois com o calculo da porcentagem vc pode acabar perdendo ou ganhando algumas  operações,
	context->qtd_operacoes = ajusta_qtd_operacoes(
				qtd_operacoes, 
				&(context->qtd_insert), 
				&(context->qtd_delete),
				&(context->qtd_search)
	);

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

	//calcula tempo em microsegundos
	tempo_total = (context->t_final.tv_sec - context->t_inicial.tv_sec) * 1000000 + 
					((int)context->t_final.tv_usec - (int)context->t_inicial.tv_usec);

	//soma todas as operações
	total_operacoes= context->cont_operacao_insert+context->cont_operacao_delete+context->cont_operacao_search;
	printf("\n._________________RESULTADOS___________________.\n");
	printf("\n Total de operações de inserão.....%ld", context->cont_operacao_insert);
	printf("\n Inserções repetidas...............%ld", context->cont_operacao_insert - context->cont_insert);
	printf("\n Total de operações de remoção.....%ld", context->cont_operacao_delete);
	printf("\n Remoções bem sucedidas............%ld", context->cont_delete);
	printf("\n Total de operações de buscas......%ld", context->cont_operacao_search);
	printf("\n");
	printf("\n Total de elementos na lista.......%ld", count(lista));
	printf("\n Total de operações realizadas.....%ld", total_operacoes);
	printf("\n");
	printf("\n.___________________TEMPO______________________.");
	printf("\n Tempo total em microsegundos......%ld", tempo_total);
	printf("\n Tempo total em segundos...........%lf", tempo_total/1000000.0);
	printf("\n Tempo total em minutos............%lf", tempo_total/1000000.0/60.0);
	printf("\n\nDeseja imprimir Lista?\n(s/n): \n");
	scanf("%s",&opcao);
	if(tolower(opcao)=='s')
		imprimir(lista);

	//libera da memória contexto dessa função
	finaliza_context(context);
	printf(PRESS_ENTER);
}







