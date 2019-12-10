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
void entradaEstoque(Produto *estoque, int *nCadastros);
void cadastrar(Produto *estoque, int* nCadastros, int *alocado, int *id);
int compID(const void *a, const void *b);
int compNome(const void *a, const void *b);
void pesquisar();
void alterar();
void exibiProduto(Produto* p);

void deletaProduto(Produto *estoque, int *nCadastros);

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
            entradaEstoque(estoque, &nCadastros);
            break;
        case 2:
            baixarEstoque(estoque, &nCadastros);
            break;
        case 3:
             cadastrar(estoque, &nCadastros, &alocado, &id);
            break;
        case 4:
            /* Pesquisar Produto*/
            break;
        case 5:
            /* alterar Produto */
            break;
        case 6:
            deletaProduto(estoque, &nCadastros);
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





int compID(const void *a, const void *b){
    Produto *x = (Produto*) a;
    Produto *y = (Produto*) b;
    return x->codigo - y->codigo;
}

int compNome(const void *a, const void *b){
    Produto *x = (Produto*) a;
    Produto *y = (Produto*) b;
    
    return strcmp(x->nome, y->nome);
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
    scanf("%s", nome);
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
    return;
        
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
        printf("cod = %d", estoque[*nCadastros].codigo);

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
    int id;
    
    printf("Digite o codigo do produto que deseja dar baixa: \n");
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);
    
    if(pVetor){
        printf("Quantas unidades deseja dar baixa?\n");
        scanf("%d",&qtdBaixa);
        printf("end pvetor  : %d", pVetor->codigo);

        if(pVetor->qtd - qtdBaixa <= 0)
            printf("Quantidade nao suportada. Estoque ficara negativo\n");
        else
            pVetor->qtd -= qtdBaixa;
    }
    else
    {
        printf("Produto nao encontrado\n");
    }    
}

void entradaEstoque(Produto *estoque, int *nCadastros){
    int qtdEntrada;
    int id;
    
    printf("Digite o codigo do produto que deseja dar baixa: \n");
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);
    
    if(pVetor){
        printf("Quantas unidades deseja dar entrada?\n");
        scanf("%d",&qtdEntrada);
        printf("end pvetor  : %d", pVetor->codigo);
        pVetor->qtd += qtdEntrada;
    }
    else
    {
        printf("Produto nao encontrado\n");
    }
    
}

void exibirEstoque(Produto *estoque, int nCadastros){
    for (int i = 0; i < nCadastros; i++)
    {
        printf("Nome do Produto: %s\n", estoque[i].nome);
        printf("Quantidade em Estoque: %d\n", estoque[i].qtd);
        printf("Codigo do produto: %d\n\n\n", estoque[i].codigo);
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

void deletaProduto(Produto *estoque, int *nCadastros){
    
    int id;
    
    
    printf("Digite o codigo do produto que deseja excluir: \n");
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);

    exibiProduto(pVetor);



    if(pVetor){
        int i = 0;          
        while(pVetor[i].codigo != estoque[(*nCadastros) - 1].codigo)             
        {   
            printf(" i = %d", i);
            pVetor[i].codigo = pVetor[i+1].codigo;
            strcpy(pVetor[i].nome, pVetor[i+1].nome);
            pVetor[i].qtd = pVetor[i+1].qtd;
            i++;
        }    
        (*nCadastros)--;

    }
    else{
        printf("Produto nao encontrado\n");
    }

}

void exibiProduto(Produto* p){
    printf("Nome do Produto: %s\n", p->nome);
    printf("Quantidade em Estoque: %d\n", p->qtd);
    printf("Codigo do produto: %d\n\n\n", p->codigo);

}