/**
* Arquivo responsável por gerenciamento manual da lista encadeada
*
* Autor: Juarez Arce Franco Junior
*/


/**
* Função responsável por gerenciamento manual da lista encadeada
*/
void manual(Node* lista){
	char opcao;
	int valor;
	Node *node_antecessor;
	system("clear");
	printf(TITLE_MANUAL);

	input_int("Quantas inserções iniciais devem ser feito na lista?\n",&valor);
	if(valor>0)
		inicializar_lista(lista, valor);
	do{
		system("clear");
		printf(TITLE_MANUAL);
		imprimir(lista);
		printf("\n(1) Inserir\n");
		printf("(2) Remover\n");
		printf("(0) Voltar\n");
		scanf("%s",&opcao);
		fflush(stdin);
		
		switch(opcao){
			case '1':
				printf("\nAntes:  ");
				imprimir(lista);
				input_int("Inserir valor: ",&valor);
				node_antecessor=buscarAntecessor(lista, valor);
				inserir(node_antecessor, valor);
				printf("Depois: ");
				imprimir(lista);
				break;
			case '2':
				printf("\nAntes:  ");
				imprimir(lista);
				input_int("Remover valor: ",&valor);
				node_antecessor=buscarAntecessor(lista, valor);
				remover(node_antecessor, valor);
				printf("Depois: ");
				imprimir(lista);
				break;
			case '0':
				break;
			default:
				printf("Opção inválida\n");
		}
		
		//Espera pressionar alguma tecla
		printf(PRESS_ENTER);
		if(opcao!='0'){
			getchar();
			fflush(stdin);
			getchar();
			fflush(stdin);
		}
	}while(opcao!='0');
}