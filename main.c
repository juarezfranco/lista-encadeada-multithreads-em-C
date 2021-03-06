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
/** FIM VARIAVEIS **/

int main (int argc, char** argv){
	char opcao;
	//system("clear");
	srand((unsigned)time(NULL));
	do{
		//cria lista
		lista = new_node();
		system("clear");
		printf(BOLDWHITE CABECALHO RESET_COLOR);
		printf("Selecione uma opção:\n");
		printf("(1) Read-write locks.\n");
		printf("(2) Um mutex para a lista inteira\n");
		printf("(3) Um mutex por nó\n");
		printf("(4) Gerenciamento Manual.\n");
		printf("(5) Calcular Tempo de Todos.\n");
		printf("(0) ++ Sair ++\n: "BOLDYELLOW);
		scanf("%s",&opcao);
		printf(RESET_COLOR);
		__fpurge(stdin);
		/** Chamada de funções de CONTROLLERS**/
		switch (opcao){
			case '1':
				start(lista, READ_WRITE);
				break;
			case '2':
				start(lista, MUTEX_ALL);
				break;
			case '3':
				start(lista, MUTEX_BY_NODE);
				break;
			case '4':
				manual(lista);
				break;
			case '5':
				start(lista, ALL);
				break;
			case '0':
				break;
			default:
				printf(BOLDRED "  Opcão inválida!\n" RESET_COLOR);	
				printf(PRESS_ENTER);
				break;	
		}
		//libera lista da memória
		finalizar_lista(lista);
		//Espera pressionar alguma tecla
		if(opcao!='0'){
			getchar();
		}
		system("clear");
	}while(opcao!='0');
	printf(RODAPE);
	return 0;
}

/**
* Função responsável por criar threads e escolher qual modo de sessão critica usar
*/
void start(Node* lista, int MODO){
	/** Variaveis locais **/
	Contexto *context = new_contexto();//cria contexto para essa função
	unsigned long total_operacoes=0; //recebera soma da qtd de operações realizadas pelas threads
	unsigned long tempo_total=0;//recerá tempo total da execução da tarefa das threads
	
	
	/** Fim variaveis locais **/

	system("clear");
	//define cabeçalho
	printf(BOLDYELLOW);
	if(MODO==READ_WRITE) 	printf(TITLE_READ_WRITE);
	if(MODO==MUTEX_ALL)		printf(TITLE_MUTEX_ALL);
	if(MODO==MUTEX_BY_NODE) printf(TITLE_MUTEX_BY_NODE);
	if(MODO==ALL) printf(TITLE_MUTEX_BY_NODE);
	printf(RESET_COLOR);

	//capitura entrada dos usuarios
	pegar_entradas_do_usuario(context);

	//captura tempo inicial
	if(MODO!=ALL)
		gettimeofday(&(context->t_inicial), NULL);

	//cria threads de acordo com modo
	if(MODO==MUTEX_ALL){
		controller_threads_mutex_all(context);
	}
	if(MODO==MUTEX_BY_NODE){
		controller_threads_mutex_by_node(context);
	}
	if(MODO==READ_WRITE){
		controller_threads_read_write(context);
	}
	//Calcula tempo de todos os modos usando as mesmas entradas do usuarios para todas as funções
	//repere que os controller estão encapsulado pelo gettimeofday. Pois cada controller deve ter seu tempo
	if(MODO==ALL){
		printf(BOLDYELLOW TITLE_MUTEX_BY_NODE RESET_COLOR);//cabeçalho
		//lista=new_node();//inicializa lista
		gettimeofday(&(context->t_inicial), NULL);
			controller_threads_mutex_by_node(context);//imprimir informações do contexto do controller_threads_mutex_by_node
		gettimeofday(&(context->t_final), NULL);//captura tempo final antes de mandar imprimir
		imprimir_contexto(context);//imprimir contexto
		resetar_contexto(context);//reseta contadores do contexto
		finalizar_lista(lista);//finaliza lista
		printf("\n\n");

		usleep(2000000);
		printf("\n\n"BOLDYELLOW TITLE_MUTEX_ALL RESET_COLOR);//cabeçalho
		lista=new_node();//inicializa lista
		gettimeofday(&(context->t_inicial), NULL);//captura tempo inicial
			controller_threads_mutex_all(context);//imprimir informações do contexto do controller_threads_mutex_all
		gettimeofday(&(context->t_final), NULL);//captura tempo final antes de mandar imprimir
		imprimir_contexto(context);//imprimir contexto
		resetar_contexto(context);//reseta contadores do contexto
		finalizar_lista(lista);//finaliza lista
		printf("\n\n");
		
		usleep(2000000);
		printf(BOLDYELLOW TITLE_READ_WRITE RESET_COLOR);//cabeçalho
		lista=new_node();//inicializa lista
		gettimeofday(&(context->t_inicial), NULL);//captura tempo inicial
			controller_threads_read_write(context);
		gettimeofday(&(context->t_final), NULL);//captura tempo final antes de mandar imprimir
		imprimir_contexto(context);//imprimir informações do contexto do controller_threads_read_write
	}else{
		//captura tempo final
		gettimeofday(&(context->t_final), NULL);
		imprimir_contexto(context);//imprimir informações do contexto
	}
	
	
	
	
	//libera da memória contexto dessa função
	finaliza_context(context);
	printf(PRESS_ENTER);
}

/**
* Função responsável por pegar entradas do usuarios
* para preencher informações iniciais do contexto.
*/
void pegar_entradas_do_usuario(Contexto *context){
	int percent_search=0, percent_insert=0, percent_delete=0;//inputs do usuario
	int qtd_operacoes,qtd_elementos_iniciais;//inputs do usuario
	char opcao;//inputs usuario


	/**Entradas do usuario**/
	//obtem qtd de threads
	do{
		input_int("\nQuantas THREADS devem ser criadas?\n: ",&(context->qtd_threads));
		if(context->qtd_threads<=0)
			printf(BOLDRED"Informe um valor acima de 0\n"RESET_COLOR);
	}while(context->qtd_threads<=0);
	//input_int("Quantas inserções iniciais devem ser feito na lista?\n: ",&qtd_elementos_iniciais);
	//inicializar_lista(lista, qtd_elementos_iniciais);
	//imprimir(lista);	
	input_int("\nQuantidade de operações que serão realizadas pelas threads?\n: ", &qtd_operacoes);
	input_percent("\nPorcentagem de buscas? \n: %",&percent_search);
	if(percent_search!=100)
		do{
			input_percent("\nPorcentagem de inserções? \n: %",&percent_insert);
		if((percent_search+percent_insert)>100)
				printf(BOLDRED"Valor inválido, informe um valor igual ou menor que %d\n" RESET_COLOR,100-percent_search);
		}while((percent_search+percent_insert)>100);
	percent_delete = 100-(percent_insert+percent_search);
	printf("\nPorcentagem de remoções: \n:"BOLDYELLOW" %%%d\n"RESET_COLOR,percent_delete);
	//salva inputs no contexto
	context->lista 			= lista;
	context->qtd_search		= round((qtd_operacoes*percent_search)/100.0);//round arredonda pra inteiro mais proximo 
	context->qtd_insert 	= round((qtd_operacoes*percent_insert)/100.0);//Para compilar precisa do parametro -lm
	context->qtd_delete 	= round((qtd_operacoes*percent_delete)/100.0);
	//ajusta qtd para ficar exata com a total de operacoes soliticadas pelo usuario,
	//pois com o calculo da porcentagem vc pode acabar perdendo ou ganhando algumas  operações,
	context->qtd_operacoes = ajusta_qtd_operacoes(
				qtd_operacoes, 
				&(context->qtd_insert), 
				&(context->qtd_delete),
				&(context->qtd_search)
	);
}

/**
* Função imprime informações do contexto
*/
void imprimir_contexto(Contexto *context){
	long tempo_total,total_operacoes;

	//calcula tempo em microsegundos
	tempo_total = (context->t_final.tv_sec - context->t_inicial.tv_sec) * 1000000 + 
					((int)context->t_final.tv_usec - (int)context->t_inicial.tv_usec);
	//soma todas as operações

	total_operacoes= context->cont_operacao_insert+context->cont_operacao_delete+context->cont_operacao_search;
	printf(BOLDWHITE"\n._________________RESULTADOS___________________.\n"RESET_COLOR);
	printf("\n Quantidade de threads.............%d" , context->qtd_threads);
	printf("\n Total de operações de inserão.....%ld", context->cont_operacao_insert);
	printf("\n Inserções repetidas...............%ld", context->cont_operacao_insert - context->cont_insert);
	printf("\n Total de operações de remoção.....%ld", context->cont_operacao_delete);
	printf("\n Remoções bem sucedidas............%ld", context->cont_delete);
	printf("\n Total de operações de buscas......%ld", context->cont_operacao_search);
	printf("\n");
	printf("\n Total de elementos na lista.......%ld", count(lista));
	printf("\n Total de operações realizadas.....%ld", total_operacoes);
	printf("\n");
	printf(BOLDWHITE"\n.___________________TEMPO______________________."RESET_COLOR);
	printf("\n Tempo total em microsegundos......%ld", tempo_total);
	printf("\n Tempo total em segundos...........%lf", tempo_total/1000000.0);
	printf("\n Tempo total em minutos............%lf", tempo_total/1000000.0/60.0);
	printf("\n");
	//printf("\n\nDeseja imprimir Lista?\n(s/n): \n");
	//scanf("%s",&opcao);
	//if(tolower(opcao)=='s')
	//	imprimir(lista);

}






