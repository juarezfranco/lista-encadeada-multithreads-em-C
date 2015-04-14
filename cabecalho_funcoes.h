/**
* Arquivo responsável por incluir os cabecalhos das
* funções principais do projeto
*
* Autor: Juarez Arce Franco Junior
*/


/************** FUNÇÕES PRINCIPAIS DA APLICAÇÃO *****************/
/**
* Função mãe das threads, responsável por iniciar contexto e fazer chamada do controller
* solicitado pelo usuário
* arquivo: main.c
*/
void start(Node* lista, int qtd_threads, int MODO);

/**
* Função responsável por criar threads e finalizálas, 
* arquivo: "controllers/mutex_all.h" 
*/
void controller_threads_mutex_all(Contexto * context);

/**
* Função responsável por criar threads e finalizálas, 
* arquivo: "controllers/mutex_by_node.h" 
*/
void controller_threads_mutex_by_node(Contexto * context);

/**
* Função responsável por criar threads e finalizálas, 
* arquivo: "controllers/read_write.h" 
*/
void controller_threads_read_write(Contexto * context);

/**
* Gerenciamento manual da lista
* arquivo: "controllers/manual.h"
*/
void manual(Node* lista);

/**
* Função escopo da thread com sessão crítica por nó
* arquivo: "controllers/mutex_all.h" 
*/
void* slave_mutex_by_node(void* args);

/**
* Função escopo da thread  com sessão critica para a lista inteira
* arquivo: "controllers/mutex_all.h" 
*/
void* slave_mutex_all(void* args);

/**
* Função escopo da thread com sessão critica read_write
* arquivo: "controllers/mutex_all.h" 
*/
void* slave_read_write(void* args);

/******* FUNÇÕES UTILITÁRIAS DO PROJETO ********/

/**
* gerador de numero randomico que simboliza operação de inserção, remoção e busca
* arquivo: "utils/util.h"
*/
int get_randomic_operacao();

/**
* Função responsável por receber entrada de nmr inteiro do usuario e valida-la
* arquivo: "utils/util.h"
*/
void input_int(char *msg,int *input);

/******* FIM FUNÇÕES ******/