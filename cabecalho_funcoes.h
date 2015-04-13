/**
* Arquivo responsável por incluir os cabecalhos das
* funções principais do projeto
*
* Autor: Juarez Arce Franco Junior
*/


/****** FUNÇÕES PRINCIPAIS (controllers) *****/
//Função mãe das threads, responsável por iniciar contexto
void start(Node* lista, long qtd_threads, int MODO);
//função responsável por criar threads e finalizálas
void manager_threads_mutex_all(Contexto * context, long qtd_threads);
//função responsável por criar threads e finalizálas
void manager_threads_mutex_all(Contexto * context, long qtd_threads);
//função responsável por criar threads e finalizálas
void manager_threads_mutex_all(Contexto * context, long qtd_threads);
//Gerenciamento manual
void manual(Node* lista);

/***** FUNÇÕES THREADS ****/
void* slave_mutex_by_node(void* args);
void* slave_mutex_all(void* args);
void* slave_read_write(void* args);

/*** FUNÇÕES UTILITARIAS **/
int get_randomic_operacao();

/******* FIM FUNÇÕES ******/