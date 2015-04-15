/**
* Arquivo responsável por conter funções utilitárias do projeto
*
* Autor: Juarez Arce Franco Junior
*/

/**
* Função responsável por receber entrada de nmr inteiro do usuario e valida-la
*/
void input_int(char *msg,int *input){
	int result=0;
	do{
		printf("%s"BOLDYELLOW,msg);	
		result = scanf("%d",input);
		__fpurge(stdin);//limpa lixo do teclado
		if(result==0)
			printf(BOLDRED "valor inválido!" RESET_COLOR);
	}while(result==0);
	printf(RESET_COLOR);
}

/**
* gerador de numero randomico que simboliza operação de inserção, remoção e busca
*/
int get_randomic_operacao(){
	int opcao;
	opcao = 1+rand()%3;
	//1 = INSERT;
	//2 = DELETE;
	//3 = SEARCH;
	return opcao;
}

/**
* Função ajusta qtd para ficar exata com a total de operacoes soliticadas pelo usuario
* pois com o calculo da porcentagem vc pode acabar perdendo ou ganhando algumas  operações, 
* consequentemente o valor total de operações geradas pela porcentagem,
* será diferente da qtd pedida pelo usuario.
* Essa função ajusta corretamente o valor de operações que devem ser feitas para ficar de acordo com
* a qtd de operações que o usuario solicitou
*/
long ajusta_qtd_operacoes(long total,long *_insert, long *_delete, long *_search){
	int val;
	long think = *_insert + *_delete + *_search;
	//se total é igual a soma das operações então está tudo correto
	if(total == think){
		return total;
	}
	//se total é maior então think deve aumentar
	if(total>think){
		do{
			//escolhe uma operação randomicamente para incrementar, até total ser igual a think
			val = get_randomic_operacao();
			if(val==INSERT){
				(*_insert)++;
			}
			if(val==DELETE){
				(*_delete)++;
			}
			if(val==SEARCH){
				(*_search)++;
			}
			think = *_insert+*_delete+*_search;
		}while(total != think);
		return think;
	}
	//se total é maior então think deve diminuir
	if(total<think){
		do{
			//escolhe uma operação randomicamente para decrementar, até total ser igual a think
			val = get_randomic_operacao();
			if(val==INSERT){
				(*_insert)--;
			}
			if(val==DELETE){
				(*_delete)--;
			}
			if(val==SEARCH){
				(*_search)--;
			}
			think = *_insert+*_delete+*_search;
		}while(total != think);
		return think;
	}
	return think;
}


/**
* Função limpa ultima linha do terminal
* parametro key, é o caracter adicionado no final da linha,
* se nao quiser adicionar nada passe '\0'
*/
void limpa_linha(char key){
     int i;//indice do caracter na linha
     int max_caracter=58;//indica o maximo de caracter que a linha pode chegar a ter, para linhas com mt texto, coloque um nmr bem maior
     printf("\r");//retorna para o inicio da linha que pretende reutilizar, isso não limpa a linha, apenas posiciona cursor ao inicio da linha

     //Agora precisamos limpar a linha,
     //substitui todos os caracteres existentes por espaço em branco
     for(i=0;i<max_caracter;i++){
        printf(" ");//vai preenchendo a linha com espaços em branco
    }
    if(key!='\0')
    	printf("%c",key);
    else
    	printf("  ");
    printf("\r");//volta ao inicio da linha , dessa vez ela está em branco.

 }

/**
* Função de carregamento animado
* 
*/
void show_loading(Contexto *context){
	int j, porcento=0,status=0;
	printf("\n");
	//condição
	//status deve ser incrementado em outra função
	while(status < context->qtd_operacoes){
		limpa_linha(']');//chama função limpa_linha e adiciona ] no final da linha
		//recupera qtd de operações ja realizada pelas threads
		status = context->cont_operacao_insert + context->cont_operacao_delete + context->cont_operacao_search;	
		//tranforma em porcentagem o status
		porcento = (status * 100) / context->qtd_operacoes;
		printf ("Processando:  %d%%    [", porcento);
        fflush (stdout);//garante a escrita de dados imediatamente na tela                  
        //repare mod 10, eu limito a qtd de pontos q serao gerados
        for (j = 0; j < porcento; j++){
        	if(j%3==0)
        		printf("=");
        }  
        printf(">");
        fflush (stdout);//garante a escrita de dados imediatamente na tela
        usleep(50000);//função espera por tempo, parametro em microsegundos.                            

    }          
    limpa_linha('\0'); //chama função limpa_linha e não adiciona nada no final da linha
    printf(BOLDGREEN"Processamento concluído 100%%\n"RESET_COLOR);

}


