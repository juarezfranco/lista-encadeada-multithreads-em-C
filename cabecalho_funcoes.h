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
void start(Node* lista, int MODO);

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

/**
* Função ajusta qtd para ficar exata com a total de operacoes soliticadas pelo usuario
* pois com o calculo da porcentagem vc pode acabar perdendo ou ganhando algumas  operações, 
* consequentemente o valor total de operações geradas pela porcentagem,
* será diferente da qtd pedida pelo usuario.
* Essa função ajusta corretamente o valor de operações que devem ser feitas para ficar de acordo com
* a qtd de operações que o usuario solicitou
* Arquivo: "utils/util.h"
*/
long ajusta_qtd_operacoes(long total_operacao,long *_insert, long *_delete, long *_search);

/**
* Função imprimi informações do contexto
* Arquivo: main.c
*/
void imprimir_contexto(Contexto *context);

/**
* Função responsável por pegar entradas do usuários
* para preencher informações iniciais do contexto.
* Arquivo: main.c
*/
void pegar_entradas_do_usuario(Contexto *context);

/******* FIM FUNÇÕES ******/