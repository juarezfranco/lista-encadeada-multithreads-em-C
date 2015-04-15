/**
* Arquivo responsável por incluir bibliotecas no projeto
*
* Autor: Juarez Arce Franco Junior
*/

//BIBLIOTECAS PADRÕES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>//NOTA: precisa do parametro -lm para compilar

//MEUS HEADERS
#include "utils/constantes.h"
#include "models/node.h"
#include "models/context.h"
#include "cabecalho_funcoes.h"
#include "utils/util.h"
#include "controllers/manual.h"
#include "controllers/mutex_all.h"
#include "controllers/mutex_by_node.h"
#include "controllers/read_write.h"

// BIBLIOTECAS PARA TRABALHAR COM THREADS
#include <sys/time.h>
#include <pthread.h>//NOTA: precisa do parametro -lpthread para compilar
