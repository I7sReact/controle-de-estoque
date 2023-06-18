// Douglas Nascimento
// André 
// Pablo



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const int MAX = 50; // Limitando o tamanho do vetor
int ordenado = 0;   // Variável para controlar a ordenação dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto{
    long int codigo;
    char nome[41];
    int quantidade;
    float prCompra;
    int grupo;
    char desc[41];
    char unidade[3];
    char fornecedor[41];
    float prVenda;
    float lucro ;
    int estoqueMin;
    struct tm dataVencido;
} Tproduto;

int dataVal(struct tm dataV);

struct tm conseguirTempoValidade(){
    struct tm dataV;
    do {
        printf("Data de validade do produto.\n");
        printf("Dia: ");
        scanf("%d", &dataV.tm_mday);
        printf("Mês: ");
        scanf("%d", &dataV.tm_mon);
        printf("Ano: ");
        scanf("%d", &dataV.tm_year);
    } while (!dataVal(dataV));
    return dataV;
}

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

void menuEsp();

void venc(Tproduto estoque[], int tamanho);

void aumentoForn(Tproduto estoque[], int tamanho);

void relatorioEstoqueMin(Tproduto estoque[], int tamanho);

void relatorioLista(Tproduto estoque[], int indice);

void menuAltVal(Tproduto estoque[], int tamanho);

// Função principal
int main(){
	
	Tproduto estoque[MAX];
    struct tm *data;
    time_t diaHoje;
    time(&diaHoje);
    data = localtime(&diaHoje);
    int tamanho=0, opc;

    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    do{
        printf("Hoje é: %d/%d/%d\n", data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
        printf (" 1 - CADASTRAR\n");
        printf (" 2 - ALTERAR\n");
        printf (" 3 - EXCLUIR\n");
        printf (" 4 - PESQUISAR\n");
    	printf (" 5 - RELATÓRIO GERAL\n");
    	printf (" 6 - RELATÓRIOS ESPECIAIS \n");
        printf (" 0 - SAIR\n");
        printf ("\nESCOLHA A OPÇÃO DESEJADA:\n");
        scanf ("%d",&opc);
        system("clear");
        switch (opc){
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
				menuEsp();		
		    	break;
            case 0:
                exit(0);
            default:
                printf("OPÇÃO INVÁLIDA!!\n\n");
                system("clear");
			}
				
			}while (opc!=0);
			    grav(estoque, tamanho);// grava após realizada as operações
				return 0;
				
}

void leitura(Tproduto estoque[], int *tamanho)
{
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   // abrimos para anexar, binário
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
    arquivo= fopen("estoque.dat", "w+b");	   // Abre e apaga o conteúdo do arquivo,binário
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);       // Fecha o arquivo de dados
	fclose(arquivo);
	return;
}

int pesqbin(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho))
       return -1;                                               // Vetor vazio       
    if (! ordenado){
        ordena(estoque,tamanho);                                // Ordena
        ordenado=1;                                             // Variável global
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio;                                         // Encontrou
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1;                                                  // Não encontrou
}

int vazio(int tamanho)
{
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
        fflush(stdin);
        getchar();
        system("clear");
        return 1;// Vazio
     }
     return 0;// Preenchido
}

void ordena(Tproduto estoque[], int tamanho)
{
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

void inclusao(Tproduto estoque[], int* tamanho){
    if (*tamanho == MAX) {
        printf("\nERRO!\nARQUIVO CHEIO.\n");
        return;
    }

    Tproduto aux;
    char correto = 'n';

    do {
        printf("Codigo: ");
        scanf("%ld", &aux.codigo);
        fflush(stdin);
    } while (aux.codigo <= 0);

    if(pesqbin(estoque, aux.codigo, *tamanho) >= 0) {
        printf("\nCODIGO JA CADASTRADO!!\n");
        printf("\nAPERTE ENTER PARA VOLTAR AO MENU\n");
        getchar();
        system("clear");
        return;
    }

    do {
        printf("Nome do produto: ");
        fgets(aux.nome, sizeof(aux.nome), stdin);
        fflush(stdin);
        aux.nome[strcspn(aux.nome, "\n")] = '\0';
        
        if (aux.nome[0] == '\0') {
            printf("O CAMPO NAO PODE SER VAZIO\n");
        }
    } while (aux.nome[0] == '\0');

    do {
        printf("Descrição do produto: ");
        fgets(aux.desc, sizeof(aux.desc), stdin);
        fflush(stdin);
        aux.desc[strcspn(aux.desc, "\n")] = '\0';
        
        if (aux.desc[0] == '\0') {
            printf("O CAMPO NAO PODE SER VAZIO\n");
        }
    } while (aux.desc[0] == '\0');

    do {
        printf("Fornecedor do produto: ");
        fgets(aux.fornecedor, sizeof(aux.fornecedor), stdin);
        fflush(stdin);
        aux.fornecedor[strcspn(aux.fornecedor, "\n")] = '\0';
        if (aux.fornecedor[0] == '\0') {
            printf("O CAMPO NAO PODE SER VAZIO\n");
        }
    } while (aux.fornecedor[0] == '\0');

    do {
        printf("Estoque minimo do produto: ");
        scanf("%d", &aux.estoqueMin);
        fflush(stdin);
    } while (aux.estoqueMin < 0);

    do {
        printf("\nGrupo : \n");
        printf("[1] - CABELO : \n");
        printf("[2] - UNHA : \n");
        printf("[3] - PELE : \n");
        printf("SELECIONE UM DESSES GRUPOS ACIMA: ");
        scanf("%d", &aux.grupo);
        fflush(stdin);

        if (aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3) {
            printf("\nGrupo inválido. Digite novamente.\n");
        }
    } while (aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3);

    do {
        printf("Quantidade do produto: ");
        scanf("%d", &aux.quantidade);
        fflush(stdin);
    } while (aux.quantidade < 0);

    do
    {
        printf("Unidade do Produto (UN ou LT): ");
        fgets(aux.unidade, sizeof(aux.unidade), stdin);
        fflush(stdin);
        aux.unidade[strcspn(aux.unidade, "\n")] = '\0';

        if (strcmp(aux.unidade, "UN") != 0 && strcmp(aux.unidade, "LT") != 0) {
            printf("Unidade inválida. Digite novamente.\n");
        }
    } while (strcmp(aux.unidade, "UN") != 0 && strcmp(aux.unidade, "LT") != 0);

    do {
        printf("Preco de compra do produto: R$");
        scanf("%f", &aux.prCompra);
        fflush(stdin);
    } while (aux.prCompra < 0);

    do
    {
        printf("Valor do Lucro minimo por produto: R$");
        scanf("%f", &aux.lucro);
        fflush(stdin);
    } while (aux.lucro < 0);

    do
    {
        printf("Preco de venda do produto: R$");
        scanf("%f", &aux.prVenda);
        fflush(stdin);
    } while (aux.prVenda < 0);

    aux.dataVencido = conseguirTempoValidade();

    printf("\nOs dados estão corretos? (S/N)");
    correto = getchar();
    fflush(stdin);
    system("clear");

    if (correto == 's' || correto == 'S'){
        estoque[*tamanho] = aux; // Faz a transferência
        (*tamanho)++;
        ordenado = 0;
        printf("\tO PRODUTO FOI INCLUIDO!\n");
    }

    printf("\nAPERTE ENTER PARA VOLTAR AO MENU\n");
    fflush(stdin);
    getchar();
    system("clear");
    return;
}

void relatorioGeral(Tproduto estoque[], int tamanho){

    struct tm *dataHoje;
    time_t diaH;
    time(&diaH);
    dataHoje = localtime(&diaH);

    int i, contador = 0;
    int limite = 2; // Define o limite de produtos a serem exibidos por página
    int pagina = 1; // Número da página atual
    int totalPaginas = (tamanho + limite - 1) / limite; // Calcula o total de páginas necessárias

    fflush(stdin);
    printf("\tControle de Estoque - Relatório geral - %d/%d/%d\n", dataHoje->tm_mday, \
    dataHoje->tm_mon + 1, dataHoje->tm_year + 1900);
    printf("=======================================================================\n");
    ordena(estoque, tamanho);
    printf("Código\t\t\t\t\tPreço\n");
    while (contador < tamanho) {
        // Exibe os produtos da página atual
        for(i = contador; i < tamanho && i < contador + limite; i++){
            printf("%ld\t\t\t\t\tR$ %.2f\n", estoque[i].codigo, estoque[i].prCompra);
            printf("Descricao: %s\n", estoque[i].desc);
            printf("Unidade: %s\t\tFornecedor: %s\n", estoque[i].unidade, estoque[i].fornecedor);
            printf("Preco de Compra: R$ %.2f\tPreco de Venda: R$ %.2f\tLucro Minimo: %.f\n", \
            estoque[i].prCompra, \
            estoque[i].prVenda, estoque[i].lucro);
            printf("Quantidade em Estoque: %d\tQuantidade Minima: %d\n", estoque[i].quantidade, \
            estoque[i].estoqueMin);
            printf("Data de validade: %d/%d/%d\n", estoque[i].dataVencido.tm_mday, \
            estoque[i].dataVencido.tm_mon, estoque[i].dataVencido.tm_year);
            printf("--------------------------------------------------------------------------\n");
        }

        printf("Página %d de %d\n", pagina, totalPaginas);
        
        if (pagina < totalPaginas) {
            printf("APERTE 'N' PARA AVAN�AR PARA A PR�XIMA P�GINA");
        }
        
        if (pagina > 1) {
            printf("APERTE 'P' PARA VOLTAR UMA P�GINA");
        }
        
        printf(" OU 'Q' PARA VOLTAR AO MENU\n");
        char opcao = getchar();
        getchar(); // Limpa o caractere de nova linha pendente

        if (opcao == 'q' || opcao == 'Q') {
            system("clear");
            break; // Sai do loop se o usu�rio escolher voltar ao menu
        } else if (opcao == 'n' || opcao == 'N') {
            if (pagina < totalPaginas) {
                contador += limite; // Avan�a para a pr�xima p�gina
                pagina++; // Atualiza o n�mero da p�gina
            }
        } else if (opcao == 'p' || opcao == 'P') {
            if (pagina > 1) {
                contador -= limite; // Volta uma p�gina
                if (contador < 0) {
                    contador = 0; // Garante que o contador n�o seja negativo
                }
                pagina--; // Atualiza o número da página
            }
        }
        
        system("clear");
    }
}

void alteracao(Tproduto estoque[], int tamanho){
    if (vazio(tamanho)) {
        return;
    }

    long int codigo;
    int posicao, opc;

    printf("Digite o codigo do produto a ser alterado: ");
    scanf("%ld", &codigo);

    posicao = pesqbin(estoque, codigo, tamanho);

    if (posicao == -1) {
        printf("Produto não encontrado!\n");
        return;
    }
    switch(opc){
    case 1:
        printf("Digite o novo grupo ao qual o produto pertence: ");
        scanf("%d", &estoque[posicao].grupo);
        break;
    case 2:
        printf("Digite a nova descrição do produto: ");
        fflush(stdin);
        fgets(estoque[posicao].desc, 41, stdin);
        estoque[posicao].desc[strcspn(estoque[posicao].desc, "\n")] = '\0';
        break;
    case 3:
        printf("Digite a nova unidade de medida: ");
        fflush(stdin);
        fgets(estoque[posicao].unidade, 3, stdin);
        estoque[posicao].unidade[strcspn(estoque[posicao].unidade, "\n")] = '\0';
        break;
    case 4:
        printf("Digite o novo nome do fornecedor do produto: ");
        fflush(stdin);
        fgets(estoque[posicao].fornecedor, 41, stdin);
        estoque[posicao].fornecedor[strcspn(estoque[posicao].fornecedor, "\n")] = '\0';
        break;
    case 5:
        printf("Digite a nova quantidade do produto em estoque: ");
        scanf("%d", &estoque[posicao].quantidade);
        break;
    case 6:
        printf("Digite o novo preco de compra do produto: ");
        scanf("%f", &estoque[posicao].prCompra);
        break;
    case 7:
        printf("Digite o novo preco de venda do produto: ");
        scanf("%f", &estoque[posicao].prVenda);
        break;
    case 8:
        printf("Digite a nova margem de lucro minima: ");
        scanf("%f", &estoque[posicao].lucro);
        break;
    case 9:
        printf("Digite a nova quantidade minima em estoque: ");
        scanf("%d", &estoque[posicao].estoqueMin);
    break;
    case 10:
        menuAltVal(estoque, tamanho);
        break;
    case 0:
        return;
        break;
    default:
        printf("OPÇÃO INVÁLIDA!!\n\n");
        system("clear");
    }
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
    printf("\n--------------------------------------------------------------------------\n");
    printf("Codigo: %ld\n", estoque[posicao].codigo);
    printf("Grupo: %d\n", estoque[posicao].grupo);
    printf("Descrição: %s\n", estoque[posicao].desc);
    printf("Unidade: %s\n", estoque[posicao].unidade);
    printf("Fornecedor: %s\n", estoque[posicao].fornecedor);
    printf("Quantidade em estoque: %.d\n", estoque[posicao].quantidade);
    printf("Preco de compra: %.2f\n", estoque[posicao].prCompra);
    printf("Preco de venda: %.2f\n", estoque[posicao].prVenda);
    printf("Margem de lucro minima: %.2f\n", estoque[posicao].lucro);
    printf("Quantidade minima em estoque: %d\n", estoque[posicao].estoqueMin);
    printf("Data de validade: %d/%d/%d\n", estoque[posicao].dataVencido.tm_mday, \
    estoque[posicao].dataVencido.tm_mon, estoque[posicao].dataVencido.tm_year);
    printf("--------------------------------------------------------------------------\n");
}

void relatorioEstoqueMin(Tproduto estoque[], int tamanho){
    
    if (tamanho == 0) {
        printf("\tEstoque vazio!\n");
        printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
	    fflush(stdin);
	    getchar();
	    system("clear");
        return;
    }

    int i;
    int encontrados = 0;
	
	if (encontrados > 0) {
		fflush(stdin);
        printf("Há produtos com quantidade em estoque abaixo do mínimo!\n\n");
    } else {
    	fflush(stdin);
        printf("Nenhum produto com quantidade em estoque abaixo do mínimo!\n\n");
    }
	
    for (i = 0; i < tamanho; i++) {
        if (estoque[i].quantidade < estoque[i].estoqueMin) {
        	printf("Produtos com quantidade em estoque abaixo do minimo:\n");
        	fflush(stdin);
            printf("\n--------------------------------------------------------------------------\n");
            printf("Codigo: %ld \n", estoque[i].codigo);
            printf("Grupo: %d\n", estoque[i].grupo);
            printf("Descricao: %s\n", estoque[i].desc);
            printf("Unidade: %s\n", estoque[i].unidade);
            printf("Fornecedor: %s\n", estoque[i].fornecedor);
            printf("Quantidade em estoque: %d\n", estoque[i].quantidade);
            printf("Preco de compra: %.2f\n", estoque[i].prCompra);
            printf("Preco de venda: %.2f\n", estoque[i].prVenda);
            printf("Margem de lucro minima: %.2f\n", estoque[i].lucro);
            printf("Quantidade minima em estoque: %d\n", estoque[i].estoqueMin);
            printf("--------------------------------------------------------------------------\n");
            printf("\n");
            encontrados++;
        }
    }

    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("clear");
    return;
}

void aumentoForn(Tproduto estoque[], int tamanho)
{

    char fornecedor[50];
    float percentualReajuste;

    printf("Informe o nome do fornecedor: ");
    fflush(stdin);
    fgets(fornecedor, 50, stdin);
    fornecedor[strcspn(fornecedor, "\n")] = '\0'; // Remove o caractere de quebra de linha

    printf("Informe o percentual de reajuste: ");
    scanf("%f", &percentualReajuste);

    int i;
    int produtosAlterados = 0;

    for (i = 0; i < tamanho; i++) {
        if (strcmp(estoque[i].fornecedor, fornecedor) == 0) 
        {
            printf("\nProduto encontrado:\n");
            printf("Codigo: %ld\n", estoque[i].codigo);
            printf("Descricao: %s\n", estoque[i].desc);
            printf("Preco de compra atual: R$ %.2f\n", estoque[i].prCompra);
            
            float novoPrecoCompra = estoque[i].prCompra * (1 + percentualReajuste / 100);
            
            printf("Novo preco de compra: R$ %.2f\n", novoPrecoCompra);
            
            printf("Deseja confirmar o aumento de preco de compra? (S/N): ");
            char opc;
            fflush(stdin);
            scanf(" %c", &opc);
            
            if (toupper(opc) == 'S') 
            {
                estoque[i].prCompra = novoPrecoCompra;
                produtosAlterados++;
                printf("Aumento de preco de compra confirmado para o produto de codigo %ld.\n", \
                estoque[i].codigo);
            }
        }
    }

    if (produtosAlterados > 0) 
    {
        printf("\nAumento de preco de compra concluido. Total de produtos alterados: %d\n", \
        produtosAlterados);
    } else 
    {
        printf("\nNenhum produto encontrado para o fornecedor informado.\n");
    }

    printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("clear");
    return;
}

void venc(Tproduto estoque[], int tamanho){

struct tm *data;
    time_t diaH;
    time(&diaH);
    data = localtime(&diaH);
    
if (tamanho == 0) {
        printf("Estoque vazio!\n");
        return;
    }

    int i;
    int encontrados = 0;

    printf("Produtos que passaram da válidade:\n");

    for (i = 0; i < tamanho; i++) 
    {
        if(estoque[i].dataVencido.tm_mday < data->tm_mday \
        && estoque[i].dataVencido.tm_mon < data->tm_mon + 1\
        && estoque[i].dataVencido.tm_year < data->tm_year + 1900 \
        || estoque[i].dataVencido.tm_year < data->tm_year + 1900){
            printf("\n--------------------------------------------------------------------------\n");
            printf("Codigo: %ld \n", estoque[i].codigo);
            printf("Grupo: %d\n", estoque[i].grupo);
            printf("desc: %s\n", estoque[i].desc);
            printf("Unidade: %s\n", estoque[i].unidade);
            printf("Fornecedor: %s\n", estoque[i].fornecedor);
            printf("Quantidade em estoque: %d\n", estoque[i].quantidade);
            printf("Preco de compra: %.2f\n", estoque[i].prCompra);
            printf("Preco de venda: %.2f\n", estoque[i].prVenda);
            printf("Margem de lucro minima: %.2f\n", estoque[i].lucro);
            printf("Quantidade minima em estoque: %d\n", estoque[i].estoqueMin);
            printf("Data de validade: %d/%d/%d\n", estoque[i].dataVencido.tm_mday, \
            estoque[i].dataVencido.tm_mon, estoque[i].dataVencido.tm_year);
            printf("--------------------------------------------------------------------------\n");
            printf("\n");
            encontrados++;
        }
    }

    if (encontrados == 0) 
    {
        printf("Nenhum produto com quantidade em estoque abaixo do mínimo.\n");
    }

    printf("APERTE ENTER PARA VOLTAR AO MENU\n");
    getchar();
    system("clear");
}

void menuEsp()
{
    
    Tproduto estoque[MAX];
    struct tm *data;
    time_t diaH;
    time(&diaH);
    data = localtime(&diaH);
    int tamanho = 0, opc;

    do 
    {
        printf("Hoje é: %d/%d/%d\n", data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
		printf("\n===== RELATÓRIOS ESPECIAIS =====\n");
		printf("1 - RELATÓRIOS DE PRODUTOS COM ESTOQUE ABAIXO DO MINÍMO : \n");
        printf("2 - AUMENTO DO PREÇO DE COMPRA DO FORNECEDOR\n");
        printf("3 - RELATÓRIO DE PRODUTOS PRÓXIMOS DA VALIDADE\n");
		printf("0 - RETORNAR AO MENU PRINCIPAL\n");
		printf("Escolha uma opção: ");
		scanf("%d", &opc);
		system("clear");
			
	    switch (opc) 
        {
		    case 1:
                relatorioEstoqueMin(estoque, tamanho);      // Faça o processamento necessário para gerar o relatório de produtos com estoque abaixo do minímo
                break;
            case 2:
                aumentoForn(estoque, tamanho);
				grav(estoque, tamanho);
                break;
            case 3:
                venc(estoque, tamanho);
                break;
			case 0:
                return;
			    break;
			default:
			    printf("OPÇÃO INVÁLIDA!!\n\n");
                system("clear");
			        }
		} while (opc != 0);
}

void menuAltVal(Tproduto estoque[], int tamanho){

    int opc;

    printf("Digite a opção para a edição da data de válidade:\n");
    printf("1 - Dia ");
    printf("2 - Mês");
    printf("3 - Ano");
    printf("4 - As 3 alternativas");
    scanf("%d", & opc);
    switch (opc){
        case 1:
            
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        default:
            printf("OPÇÃO INVÁLIDA!!\n\n");
    }
}

void relatorioLista(Tproduto estoque[], int indice) {
    printf("%ld\t\t\t\t%.2f\n", estoque[indice].codigo, estoque[indice].prVenda);
}

int dataVal(struct tm dataV) {
    // Checar se a data é valida
    if (dataV.tm_year < 1900){
        printf("DATA INVÁLIDA\n!!");
        return 0;
    }
    if (dataV.tm_mon < 1 || dataV.tm_mon > 12){
        printf("DATA INVÁLIDA\n!!");
        return 0;
    }
    if (dataV.tm_mday < 1 || dataV.tm_mday > 31){
        printf("DATA INVÁLIDA!!\n");
        return 0;
    }
    return 1;
}