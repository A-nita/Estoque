#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __unix__         
    #include <unistd.h>
    #include <stdlib.h>

#elif defined(_WIN32) || defined(WIN32) 

   #define OS_Windows

   #include <windows.h>

#endif

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
void pesquisarProduto(Produto *estoque, int *nCadastros);

void alterarProduto(Produto *estoque, int *nCadastros);

void exibiProduto(Produto* p);
void deletaProduto(Produto *estoque, int *nCadastros);
void listarNome(Produto *estoque, int nCadastros);
void listarId(Produto *estoque, int nCadastros);
Produto *abrirArquivo(int* nCadastros, int*alocado, int *id);
void salvarArquivo(Produto *estoque, int nCadastros);
void exibirEstoque(Produto *estoque, int nCadastros);
void sair();



int main(){
    int nCadastros;
    int alocado;
    int id;
    Produto *estoque;
    int opMenu = 8;

    estoque = abrirArquivo(&nCadastros, &alocado, &id);

    do
    {
        limparTela();
        printf("\n\t\tControle de Estoque\t\t\n\n");

        printf("\n Entrada de Estoque         1\n");
        printf("\n Saida de Estoque           2\n");
        printf("\n Cadastrar Produto          3\n");
        printf("\n Pesquisar Produto          4\n");
        printf("\n Alterar Produto            5\n");
        printf("\n Excluir Produto            6\n");
        printf("\n Listar Produtos por Id     7\n");
        printf("\n Listar Produtos por nome   8\n");
       printf("\n Sair                       9\n");

       printf("\nDigite uma opcao : \n");
       scanf("%d", &opMenu);
       
        switch (opMenu)
        {
        case 1:
            limparTela();
            entradaEstoque(estoque, &nCadastros);
            break;
        case 2:
            limparTela();
            baixarEstoque(estoque, &nCadastros);
            break;
        case 3:
            limparTela();
            cadastrar(estoque, &nCadastros, &alocado, &id);
            break;
        case 4:
            limparTela();
            pesquisarProduto(estoque, &nCadastros);
            break;
        case 5:
            limparTela();
            alterarProduto(estoque, &nCadastros);
            break;
        case 6:
            limparTela();
            deletaProduto(estoque, &nCadastros);
            break;
        case 7:
            limparTela();
            listarId(estoque, nCadastros);            
            break;
        case 8:
            limparTela();
            listarNome(estoque, nCadastros);            
            break;
        }
        
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
        #ifdef OS_Windows
    
        system("cls");
    #else
    
        system("clear");
    #endif  
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

    printf("Digite a quantidade do produto em estoque: \n");

    scanf("%d", &estoque[*nCadastros].qtd);
    estoque[*nCadastros].codigo = (*id) + 1;  

    limparTela();
    exibiProduto(&estoque[(*nCadastros)]);
    
    (*nCadastros)++;
    (*id)++;

    sair();
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
        *id =  estoque[(*nCadastros)--].codigo;
        printf("cod = %d", estoque[(*nCadastros)--].codigo);

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
        exibiProduto(pVetor);

        printf("Quantas unidades deseja dar baixa?\n");
        scanf("%d",&qtdBaixa);
        

        if(pVetor->qtd - qtdBaixa <= 0)
            printf("\nQuantidade nao suportada. Estoque ficara negativo\n");
        else
            pVetor->qtd -= qtdBaixa;
            
            limparTela();
            exibiProduto(pVetor);
    }
    else
    {
        printf("Produto nao encontrado\n");
    }    

    sair();
}

void entradaEstoque(Produto *estoque, int *nCadastros){
    int qtdEntrada;
    int id;
    
    printf("Digite o codigo do produto que deseja dar entrada: \n");
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);
    
    if(pVetor){
        exibiProduto(pVetor);
        printf("Quantas unidades deseja dar entrada?\n");
        scanf("%d",&qtdEntrada);
        pVetor->qtd += qtdEntrada;

        limparTela();
        exibiProduto(pVetor);
    }
    else
    {
        printf("Produto nao encontrado\n");
    }

    sair();
    
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

    sair();
}

void listarNome(Produto *estoque, int nCadastros){
    qsort(estoque, nCadastros, sizeof(Produto), compNome);
    exibirEstoque(estoque, nCadastros);

    sair();
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

    sair();
}

void exibiProduto(Produto* p){
    printf("Nome do Produto: %s\n", p->nome);
    printf("Quantidade em Estoque: %d\n", p->qtd);
    printf("Codigo do produto: %d\n\n\n", p->codigo);    
}

void alterarProduto(Produto *estoque, int *nCadastros){
    int id;
    
    printf("Digite o codigo do produto que deseja alterar o nome: \n");
    getchar();
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);
    if(pVetor){
        exibiProduto(pVetor);
        char nome[TAM_NOME];
        printf("Digite o novo nome do produto: \n");
        scanf("%s", nome);
        strcpy(pVetor->nome, nome);
        printf("Novo nome: %s\n", pVetor->nome);
    }
    else{
        printf("Produto nao encontrado\n");
    }
}

void pesquisarProduto(Produto *estoque, int *nCadastros){
    
    int id;    
    
    printf("Digite o codigo do produto que deseja pesquisar: \n");
    scanf("%d", &id);

    Produto *pVetor = (Produto*) bsearch(&id, estoque, *nCadastros, sizeof(Produto), compID);

    exibiProduto(pVetor);
    sair();
}

void sair(){
    char sair;
    printf("\nPressione algum botao para sair\n");
    getchar();
    scanf("%c", &sair);
}
