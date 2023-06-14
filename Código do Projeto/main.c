#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

const int MAX = 50; // limitando o tamanho do vetor
int ordenado = 0;   // variável p/ controlar a ordenação dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto
{
    long int codigo;
    int quantidade;
    float prCompra;
    int grupo;
    char descricao[41];
    char unidade[3];
    char fornecedor[41];
    float prVenda;
    float lucro ;
    int estoqueMin;
    char validade[8];
} Tproduto;

// Função para leitura dos dados do arquivo
void leitura(Tproduto estoque[], int *tamanho);

// Função para gravação dos dados no arquivo
void grav(Tproduto estoque[], int tamanho);

// Função de pesquisa binária
int pesqbin(Tproduto estoque[], int chave, int tamanho);

// Função auxiliar para verificar se o vetor está vazio
int vazio(int tamanho);

// Função para ordenar o vetor de produtos por código
void ordena(Tproduto estoque[], int tamanho);

// Função para inclusão de um novo produto
void inclusao(Tproduto estoque[], int *tamanho);

// Função para exibir o relatório geral de produtos
void relatorioGeral(Tproduto estoque[], int tamanho);

// Função para realizar a alteração de um produto existente
void alteracao(Tproduto estoque[], int tamanho);

// Função para excluir um produto do estoque
void exclusao(Tproduto estoque[], int *tamanho);

// Função para realizar uma consulta de um produto
void consulta(Tproduto estoque[], int tamanho);

// Função para realizar uma consulta de produtos vencidos
void venc(Tproduto estoque[], int tamanho);



// Função principal
int main(){
	
	Tproduto estoque[MAX];
    int tamanho=0, opcao;
    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    do{
        printf (" 1- CADASTRAR\n");
        printf (" 2- ALTERAR\n");
        printf (" 3- EXCLUIR\n");
        printf (" 4- PESQUISAR\n");
    	printf (" 5- RELATÓRIO GERAL\n");
    	printf (" 6- RELATÓRIOS ESPECIAIS \n");
        printf (" 0- SAIR\n");
        printf ("\nESCOLHA A OPÇÃO DESEJADA:\n");
        scanf ("%d",&opcao);
        system("clear");
        switch (opcao){
            case 1:
            	inclusao(estoque, &tamanho);
            	grav(estoque, tamanho);
				break;
			case 2 :
				alteracao(estoque, tamanho);
                grav(estoque, tamanho);
				break;
			case 3 :
				exclusao(estoque, &tamanho);
                grav(estoque, tamanho);
				break;
			case 4 :
				consulta(estoque, tamanho);
				break;
            case 5:
            	relatorioGeral(estoque,  tamanho);
				break;
			case 6:
				do {
			    	int opcao;
			        printf("\n===== RELATÓRIOS ESPECIAIS =====\n");
			        printf("1 - RELATÓRIOS DE PRODUTOS COM ESTOQUE ABAIXO DO MINÍMO : \n");
                    printf("2 - AUMENTO DO PREÇO DE COMPRA DO FORNECEDOR\n");
                    printf("3 - RELATÓRIO DE PRODUTOS PRÓXIMOS DA VALIDADE\n");
			        printf("0 - RETORNAR AO MENU PRINCIPAL\n");
			        printf("Escolha uma opção: ");
			        scanf("%d", &opcao);
			        system("clear");
			
			        switch (opcao) {
			            case 1:
                            relatorioEstoqueMin(estoque, tamanho);
			                // Faça o processamento necessário para gerar o relatório de produtos com estoque abaixo do minímo
                        case 2:

                            break;
                        case 3:
                        
                            break;
			            case 0:
			                printf("Obrigado por usar nosso sistema.\n");
			                // system("pause");  // Não é necessário em todas as plataformas
			                // exit(0);  // Não é necessário se você estiver retornando de uma função
			                break;
			            default:
			                printf("OPÇÃO INVÁLIDA!\n");
			                // system("pause");  // Não é necessário em todas as plataformas
			                // system("clear");  // Não é necessário em todas as plataformas
			        }
					
			    } while (opcao != 0);
							
		    	break;
			}
				
			}while (opcao!=0);
			    grav(estoque, tamanho);// grava após realizada as operações
				return 0;
				
}
void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, binário */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
//le os dados do arquivo
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (Tproduto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
//fecha o arquivo
	fclose(arquivo);
	return;
}
void grav(Tproduto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conteúdo do arquivo,binário */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);
// Fecha o arquivo de dados
	fclose(arquivo);
	return;
}
int pesqbin(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho))
       return -1;// vetor vazio       
    if (! ordenado){
        ordena(estoque,tamanho);// ordena
        ordenado=1;// variavel global
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio; // encontrou
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1; // não encontrou
}
int vazio(int tamanho){
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        return 1;// vazio
     }
     return 0;// preenchido
}
void ordena(Tproduto estoque[], int tamanho){
	int i,j;
    Tproduto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].codigo>estoque[j].codigo){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}
void inclusao(Tproduto estoque[], int* tamanho) {
    if (*tamanho == MAX) {
        printf("\nERRO!\nARQUIVO CHEIO.\n");
        return;
    }

    Tproduto aux;
    char correto = 'n';
    do {
        printf("Codigo ..............................: ");
        scanf("%ld", &aux.codigo);
        fflush(stdin);
    } while (aux.codigo <= 0);

    if (pesqbin(estoque, aux.codigo, *tamanho) >= 0) {
        printf("\nCODIGO JA CADASTRADO!!\n");
        printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
        getchar();
        system("clear");
        return;
    }

    do {
        printf("Descricao do produto ..........: ");
        fgets(aux.descricao, sizeof(aux.descricao), stdin);
        fflush(stdin);
        aux.descricao[strcspn(aux.descricao, "\n")] = '\0';
        if (aux.descricao[0] == '\0') {
            printf("O CAMPO NAO PODE SER VAZIO\n");
        }
    } while (aux.descricao[0] == '\0');

    do {
        printf("Fornecedor do produto ..........: ");
        fgets(aux.fornecedor, sizeof(aux.fornecedor), stdin);
        fflush(stdin);
        aux.fornecedor[strcspn(aux.fornecedor, "\n")] = '\0';
        if (aux.fornecedor[0] == '\0') {
            printf("O CAMPO NAO PODE SER VAZIO\n");
        }
    } while (aux.fornecedor[0] == '\0');

    do {
        printf("Estoque minimo do produto ..........: ");
        scanf("%d", &aux.estoqueMin);
        fflush(stdin);
    } while (aux.estoqueMin < 0);

    do {
        printf("Grupo : \n");
        printf("[1] - CABELO : \n");
        printf("[2] - UNHA : \n");
        printf("[3] - PELE : \n");
        printf("SELECIONE UM DESSES GRUPOS ACIMA ..............: ");
        scanf("%d", &aux.grupo);
        fflush(stdin);

        if (aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3) {
            printf("Grupo inválido. Digite novamente.\n");
        }
    } while (aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3);

    do {
        printf("Quantidade do produto ...............: ");
        scanf("%d", &aux.quantidade);
        fflush(stdin);
    } while (aux.quantidade < 0);

    do {
        printf("Unidade do Produto (UN e LT) ..........: ");
        fgets(aux.unidade, sizeof(aux.unidade), stdin);
        fflush(stdin);
        aux.unidade[strcspn(aux.unidade, "\n")] = '\0';

        if (strcmp(aux.unidade, "UN") != 0 && strcmp(aux.unidade, "LT") != 0) {
            printf("Unidade inválida. Digite novamente.\n");
        }
    } while (strcmp(aux.unidade, "UN") != 0 && strcmp(aux.unidade, "LT") != 0);

    do {
        printf("Preco de compra do produto ..........: ");
        scanf("%f", &aux.prCompra);
        fflush(stdin);
    } while (aux.prCompra < 0);

    do {
        printf("Valor do Lucro minimo por produto: ");
        scanf("%f", &aux.lucro);
        fflush(stdin);
    } while (aux.lucro < 0);

    do {
        printf("Preco de venda do produto ..........: ");
        scanf("%f", &aux.prVenda);
        fflush(stdin);
    } while (aux.prVenda < 0);

    printf("\nOs dados estao corretos? (S/N)");
    correto = getchar();
    fflush(stdin);
    system("clear");

    if (correto == 's' || correto == 'S') {
        estoque[*tamanho] = aux; // faz a transferencia
        (*tamanho)++;
        ordenado = 0;
        printf("\tO PRODUTO FOI INCLUIDO!\n");
    }

    printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("clear");
    return;
}

void relatorioGeral(Tproduto estoque[], int tamanho) {
    int i, contador = 0;
    printf("\t\tControle de Estoque - Relatório geral\n");
    printf("=======================================================================\n");
    ordena(estoque, tamanho);
    printf("Código\t\t\t\t\tPreço\n");
    for (i = 0; i < tamanho; i++) {
        printf("%ld\t\t\t\t\t\t%.2f\n", estoque[i].codigo, estoque[i].prCompra);
        printf("Descrição: %s\n", estoque[i].descricao);
        printf("Unidade: %s\t\tFornecedor: %s\n", estoque[i].unidade, estoque[i].fornecedor);
        printf("Preço de Compra: %.2f\tPreço de Venda: %.2f\tLucro Mínimo: %f\n", estoque[i].prCompra, estoque[i].prVenda, estoque[i].lucro);
        printf("Quantidade em Estoque: %d\tQuantidade Mínima: %d\n", estoque[i].quantidade, estoque[i].estoqueMin);
        printf("--------------------------------------------------------------------------\n");

        contador++;
        if (contador == 2) {
            fflush(stdin);
            printf("APERTE ENTER PARA VER MAIS RELATÓRIOS");
            getchar();
            system("clear");
            contador = 0;
            printf("\t\tControle de Estoque - Relatório geral\n");
            printf("=======================================================================\n");
            printf("Código\t\t\t\t\tPreço\n");
        }
    }
    fflush(stdin);
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("clear");
}
void alteracao(Tproduto estoque[], int tamanho) {
    if (vazio(tamanho)) {
        return;
    }

    long int codigo;
    int posicao;

    printf("Digite o codigo do produto a ser alterado: ");
    scanf("%ld", &codigo);

    posicao = pesqbin(estoque, codigo, tamanho);

    if (posicao == -1) {
        printf("Produto nao encontrado!\n");
        return;
    }

    printf("Digite o novo grupo ao qual o produto pertence: ");
    scanf("%d", &estoque[posicao].grupo);

    printf("Digite a nova descricao do produto: ");
    fflush(stdin);
    fgets(estoque[posicao].descricao, 41, stdin);
    estoque[posicao].descricao[strcspn(estoque[posicao].descricao, "\n")] = '\0';

    printf("Digite a nova unidade de medida: ");
    fflush(stdin);
    fgets(estoque[posicao].unidade, 3, stdin);
    estoque[posicao].unidade[strcspn(estoque[posicao].unidade, "\n")] = '\0';

    printf("Digite o novo nome do fornecedor do produto: ");
    fflush(stdin);
    fgets(estoque[posicao].fornecedor, 41, stdin);
    estoque[posicao].fornecedor[strcspn(estoque[posicao].fornecedor, "\n")] = '\0';

    printf("Digite a nova quantidade do produto em estoque: ");
    scanf("%d", &estoque[posicao].quantidade);

    printf("Digite o novo preco de compra do produto: ");
    scanf("%f", &estoque[posicao].prCompra);

    printf("Digite o novo preco de venda do produto: ");
    scanf("%f", &estoque[posicao].prVenda);

    printf("Digite a nova margem de lucro minima: ");
    scanf("%f", &estoque[posicao].lucro);

    printf("Digite a nova quantidade minima em estoque: ");
    scanf("%d", &estoque[posicao].estoqueMin);
}
void exclusao(Tproduto estoque[], int* tamanho) {
    if (vazio(*tamanho)) {
        return;
        
    }

    long int codigo;
    int posicao, i;

    printf("Digite o codigo do produto a ser excluido: ");
    scanf("%ld", &codigo);

    posicao = pesqbin(estoque, codigo, *tamanho);

    if (posicao == -1) {
        printf("Produto nao encontrado!\n");
        return;
    }

    for (i = posicao; i < (*tamanho) - 1; i++) {
        estoque[i] = estoque[i + 1];
    }
    

    (*tamanho)--;
    ordenado = 0;
    printf("\n ITEM EXCLUIDO COM SUCESSO !... \n ");
    system("\n pause \n");
    system("clear");
}
void consulta(Tproduto estoque[], int tamanho) {
    if (vazio(tamanho)) {
        return;
    }

    long int codigo;
    int posicao;

    printf("Digite o codigo do produto a ser consultado: ");
    scanf("%ld", &codigo);

    posicao = pesqbin(estoque, codigo, tamanho);

    if (posicao == -1) {
        printf("Produto nao encontrado!\n");
        return;
    }

    printf("Codigo: %ld\n", estoque[posicao].codigo);
    printf("Grupo: %d\n", estoque[posicao].grupo);
    printf("Descricao: %s\n", estoque[posicao].descricao);
    printf("Unidade: %s\n", estoque[posicao].unidade);
    printf("Fornecedor: %s\n", estoque[posicao].fornecedor);
    printf("Quantidade em estoque: %.d\n", estoque[posicao].quantidade);
    printf("Preco de compra: %.2f\n", estoque[posicao].prCompra);
    printf("Preco de venda: %.2f\n", estoque[posicao].prVenda);
    printf("Margem de lucro minima: %.2f\n", estoque[posicao].lucro);
    printf("Quantidade minima em estoque: %d\n", estoque[posicao].estoqueMin);
}

void relatorioEstoqueMin(Tproduto estoque[], int tamanho) {
    if (tamanho == 0) {
        printf("Estoque vazio!\n");
        return;
    }

    int i;
    int encontrados = 0;
    printf("Produtos com quantidade em estoque abaixo do mínimo:\n");

    for (i = 0; i < tamanho; i++) {
        if (estoque[i].quantidade < estoque[i].estoqueMin) {
            printf("Codigo: %ld \n", estoque[i].codigo);
            printf("Grupo: %d\n", estoque[i].grupo);
            printf("Descricao: %s\n", estoque[i].descricao);
            printf("Unidade: %s\n", estoque[i].unidade);
            printf("Fornecedor: %s\n", estoque[i].fornecedor);
            printf("Quantidade em estoque: %d\n", estoque[i].quantidade);
            printf("Preco de compra: %.2f\n", estoque[i].prCompra);
            printf("Preco de venda: %.2f\n", estoque[i].prVenda);
            printf("Margem de lucro minima: %.2f\n", estoque[i].lucro);
            printf("Quantidade minima em estoque: %d\n", estoque[i].estoqueMin);
            printf("\n");
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum produto com quantidade em estoque abaixo do mínimo.\n");
    }

    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("clear");
}

void aumentForn(){

}

void venc(Tproduto estoque[], int tamanho){


}
