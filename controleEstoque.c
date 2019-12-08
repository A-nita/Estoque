#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_NOME 50

typedef struct{
    int codigo;
    char nome [TAM_NOME];
    int qtd;
}Produto;


void limparTela();

void baixarEstoque(Produto *estoque, int *nCadastros);

void cadastrar(Produto *estoque, int* nCadastros, int *alocado, int *id);
int compID(void *a, void *b);
int compNome(void *a, void *b);
void pesquisar();
void alterar();
void excluir();
void listarNome(Produto *estoque, int nCadastros);
void listarId(Produto *estoque, int nCadastros);
Produto *abrirArquivo(int* nCadastros, int*alocado, int *id);
void salvarArquivo(Produto *estoque, int nCadastros);
void exibirEstoque(Produto *estoque, int nCadastros);



int main(){
    int nCadastros;
    int alocado;
    int id;
    Produto *estoque;
    int opMenu = 8;

    estoque = abrirArquivo(&nCadastros, &alocado, &id);
    


    do
    {
       printf("\n################Controle de Estoque################");

        printf("\n Entrada de Estoque         1\n");
        printf("\n Saida de Estoque           2\n");
        printf("\n Cadastrar Produto          3\n");
        printf("\n Pesquisar Produto          4\n");
        printf("\n Alterar Produto            5\n");
        printf("\n Excluir Produto            6\n");
        printf("\n Listar Produtos por Id     7\n");
        printf("\n Listar Produtos por nome   8\n");
       printf("\n Sair                       9\n");

       printf("Digite uma opcao : \n");
       scanf("%d", &opMenu);
       //limparTela();
        switch (opMenu)
        {
        case 1:
            /* code */
            break;
        case 2:
            baixarEstoque(estoque, nCadastros);
            break;
        case 3:
             cadastrar(estoque, &nCadastros, &alocado, &id);
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            listarId(estoque, nCadastros);            
            break;
        case 8:
            listarNome(estoque, nCadastros);            
            break;
        }
        // limparTela();

    } while (opMenu != 9);  

    salvarArquivo(estoque, nCadastros);

    free(estoque);

    return 0;
}






void limparTela(){

    for (int i = 0; i < 15; i++)
    {
        printf("\n");
    }    
}

void cadastrar(Produto *estoque, int* nCadastros, int *alocado, int *id){    
      
    if(*nCadastros == *alocado){
        Produto* novoEstoque;
        novoEstoque = (Produto*) realloc(estoque, sizeof(Produto) * (*alocado * 2));

        if(novoEstoque){
            estoque = novoEstoque;
        }
        else
        {
            printf("Erro de alocacao");
        }
        
        *alocado += *alocado;               
    }

    char nome[TAM_NOME];

    printf("Digite o nome do produto:\n");
    scanf("%s",&nome );
    strcpy(estoque[*nCadastros].nome, nome);

    printf("Nome Est: %s\n", estoque[*nCadastros].nome);
    printf("Digite a quantidade do produto em estoque: \n");

    scanf("%d", &estoque[*nCadastros].qtd);
    estoque[*nCadastros].codigo = (*id) + 1;    
    
    printf("estoque[*nCadastros].nome: %s\n", estoque[*nCadastros].nome);
    printf("estoque[*nCadastros].qtd: %d\n", estoque[*nCadastros].qtd);
    printf("estoque[*nCadastros].codigo: %d\n", estoque[*nCadastros].codigo);

    (*nCadastros)++;
    (*id)++;
    limparTela();
    return estoque;
        
}

Produto *abrirArquivo(int* nCadastros, int*alocado, int *id){

    Produto *estoque;    
    FILE *file = fopen("estoque.dat","r");

    if(file==NULL){
        *nCadastros = 0;
        *alocado = 20;
        *id = 0;

        estoque = (Produto*) malloc(sizeof(Produto) * (*alocado));
    }
    else{
        fread(nCadastros, sizeof(int), 1, file);
        *alocado = (*nCadastros) * 2; 
        *id =  estoque[*nCadastros].codigo;
        printf("cod = %d", &estoque[*nCadastros].codigo);

        Produto *estoque = (Produto*) malloc(sizeof(Produto) * (*alocado));

        fread(estoque, sizeof(Produto), *nCadastros, file);
        fclose(file);
    }

    return estoque;
}
 
void salvarArquivo(Produto *estoque, int nCadastros){

    FILE *file = fopen("estoque.dat", "w");

    fwrite(&nCadastros, sizeof(int), 1, file);

    if (nCadastros > 0){

        fwrite(estoque, sizeof(Produto), nCadastros, file);
    }
    fclose(file);
}

void baixarEstoque(Produto *estoque, int *nCadastros){
    int qtdBaixa;
    char id;
    int pVetor = -1;
    printf("Digite o codigo do produto que deseja dar baixa: \n");
    scanf("%d", &id);
    qsort(estoque, nCadastros,  sizeof(Produto), compID);

    pVetor = bsearch(id, estoque, nCadastros, sizeof(Produto), compID);
    
    if(pVetor != -1){
        printf("Quantas unidades deseja dar baixa?\n");
        scanf("%d",&qtdBaixa);
        if(estoque[pVetor].qtd - qtdBaixa <= 0)
            printf("Quantidade nao suportada. Estoque ficara negativo\n");
        else
            estoque[pVetor].qtd -= qtdBaixa;
    }
    else
    {
        printf("Produto nao encontrado\n");
    }
    
}

int compID(void *a, void *b){
    Produto *x = (Produto*) a;
    Produto *y = (Produto*) b;
    return x->codigo - y->codigo;
}

void exibirEstoque(Produto *estoque, int nCadastros){
    for (int i = 0; i < nCadastros; i++)
    {
        printf("estoque[i].nome: %s\n", estoque[i].nome);
        printf("estoque[i].qtd: %d\n", estoque[i].qtd);
        printf("estoque[i].codigo: %d\n\n\n", estoque[i].codigo);
    }
}

void listarId(Produto *estoque, int nCadastros){
    qsort(estoque, nCadastros, sizeof(Produto), compID);
    exibirEstoque(estoque, nCadastros);
}

void listarNome(Produto *estoque, int nCadastros){
    qsort(estoque, nCadastros, sizeof(Produto), compNome);
    exibirEstoque(estoque, nCadastros);
}

int compNome(void *a, void *b){
    Produto *x = (Produto*) a;
    Produto *y = (Produto*) b;
    
    return strcmp(x->nome, y->nome);
}