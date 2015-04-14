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
		printf("%s",msg);	
		result = scanf("%d",input);
		__fpurge(stdin);//limpa lixo do teclado
		if(result==0)
			printf("valor inválido! entre novamente com um numero.\n");
	}while(result==0);
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