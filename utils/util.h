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
	printf("think %ld\n", think);
	//se total é igual a soma das operações então está tudo correto
	if(total == think){
		printf("%ld nao mudou\n", think);
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
		printf("%ld após incremento\n", think);
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
		printf("%ld após decremento\n", think);
		return think;
	}
	return think;
}