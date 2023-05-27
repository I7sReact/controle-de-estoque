#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX=50;
int ordenado=0;

typedef struct TProduto{
    long int codigo;
    int grupo;
    char desc[100];
    char unidade[15];
    char fonecedor[30];
    float quantidade;
    float prCompra;
    float prVenda;
    int lucro;
    float estoqueMinimo;
}Tproduto;

void leitura(Tproduto estoque[], int *tamanho);
void grav(Tproduto estoque[], int tamanho);
void excl(Tproduto estoque[], int tamanho);
int pesquisabi(Tproduto estoque[], int chave, int tamanho);
int vazio (int tamanho);
void ordena(Tproduto estoque[], int tamanho);
void inclusao(Tproduto estoque[],int *tamanho);
void relatorioGeral (Tproduto estoque[], int tamanho);
void ;

int main(){
	Tproduto estoque[MAX];
    int tamanho=0, opc;
    leitura(estoque, &tamanho);

    do{
        printf ("\n 1- CADASTRAR\n");
    	printf (" 2- ALTERAR\n");
        printf (" 3- EXCLUIR\n");
        printf (" 4- CONSULTA\n");
        printf (" 5- RELATÓRIO GERAL\n");
        printf (" 6- PRODUTOS COM ESTOQUE ABAIXO\n");
        printf (" 7- AUMENTO DO PREÇO DO FORNECEDOR\n");
        printf (" 0- SAIR\n");
        printf ("\nESCOLHA A OPCAO DESEJADA:\n");
        scanf ("%d",&opc);
        system("clear");
        switch (opc){
            case 1:{
            	inclusao(estoque, &tamanho);
            	grav(estoque, tamanho);
				break;
			}        	
            case 2:{
				relatorioGeral (estoque, tamanho);
				break;
			}
            case 3:{

                break;
            }
            case 4:{
                pesquisabi(estoque, chave, tamanho);
                break;
            }
            case 5:{
                relatorioGeral (estoque, tamanho);
                break;
            }
            case 6:{

                break;
            }
            case 7:{

                break;
            }
            case 0: 
				printf("Obrigado por usar nosso sistema.\n");
				system("pause");
				exit(0);
				break;
            default: printf("OPCAO INVALIDA!\n");
            	system("pause");
            	system("clear");
        }
    }while (opc!=0);
        gravacao(estoque, tamanho);
	return 0;
}

void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (Tproduto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
	fclose(arquivo);
	return;
}

void gravacao(Tproduto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b"); 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);
	fclose(arquivo);
	return;
}

void excl(Tproduto estoque[], int tamanho){
    FILE *arquivo;

    arquivo= fopen("estoque.dat", "w+b");

    int codigo;
    int indice = -1;

    printf ("Digite o código do produto a ser excluído: ");
    scanf ("%d", &codigo);

    for (int i = 0; i < tamanho; i++) {
        if (codigos[i] == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf ("Produto não encontrado.\n");
        return;
    }

    for (int i = indice; i < tamanho - 1; i++) {
        codigos[i] = codigos[i + 1];
        strcpy(nomes[i], nomes[i + 1]);
        precos[i] = precos[i + 1];
    }

    numProdutos--;

    printf("PRODUTO EXCLUÍDO COM SUCESSO!\n");
}

int pesquisabi(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho))
       return -1;      
    if (! ordenado){
        ordena(estoque,tamanho);
        ordenado=1;
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio;
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1;
}

int vazio(int tamanho){
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        return 1;
     }
     return 0;
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

void inclusao(Tproduto estoque[],int *tamanho){
             if (*tamanho==MAX){ 
                   printf ("\n ERRO! \n ARQUIVO CHEIO.\n");
                   return;
             }
             Tproduto aux;
             char correto='n';
             do{
                  printf ("Codigo ..............................: ");
                  scanf ("%ld",&aux.codigo);
				  fflush(stdin);
            }while (aux.codigo <= 0);
            if (pesquisabinaria(estoque, aux.codigo, *tamanho) <0){
                  do{
                        printf ("Quantidade do produto ...............: ");
                        scanf ("%f",&aux.quantidade);
						fflush(stdin);
                    }while(aux.quantidade<0);
                  do{
                  		printf ("Preco de compra do produto ..........: ");
                  		scanf ("%f",&aux.prCompra);
						fflush(stdin);
				  }while(aux.prCompra<0);
				  
                  
				  printf ("Os dados estao corretos?(S/N)");
                  correto=getchar();
				  fflush(stdin);
				  system("clear");
                  
                  if (correto=='s'||correto=='S'){
                       estoque [*tamanho] = aux;
                       (*tamanho) ++;
                       ordenado=0;
                       printf ("\tO PRODUTO FOI INCLUIDO!\n");
					fflush(stdin);
                  }
			}else 
    			printf ("\tCODIGO JA CADASTRADO!!\n");
    printf("\tAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("clear");
    return;
}

void relatorioGeral (Tproduto estoque[], int tamanho){
	int i;
	printf("\t\tControle de Estoque - Relatorio geral\n");
    printf("=======================================================================\n");
    ordena(estoque, tamanho);
    printf("Codigo\t\t\t\t\tPreco\n");
    for(i=0; i<tamanho; i++)
    	printf("%ld\t\t\t\t\t\t%.2f\n",estoque[i].codigo,estoque[i].prCompra);
     printf("-----------------------------------------------------------------------\n\n");
    fflush(stdin);
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("clear");
}
