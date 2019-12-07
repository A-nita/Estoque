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
void editarEstoque();

void cadastrar(Produto *estoque, int* nCadastros, int *alocado, int *id);

void pesquisar();
void alterar();
void excluir();
void listarNome();
void listarCod();
Produto *abrirArquivo(int* nCadastros, int*alocado, int *id);
void salvarArquivo(Produto *estoque, int nCadastros);




int main(){
    int nCadastros;
    int alocado;
    int id;
    Produto *estoque;
    int opMenu = 8;

    estoque = abrirArquivo(&nCadastros, &alocado, &id);
    


    do
    {
       printf("\n################Controle de Estoque################\n");

       printf("\n Entrada de Estoque  1\n");
       printf("\n Saida de Estoque    2\n");
       printf("\n Cadastrar Produto   3\n");
       printf("\n Pesquisar Produto   4\n");
       printf("\n Alterar Produto     5\n");
       printf("\n Excluir Produto     6\n");
       printf("\n Listar Produtos     7\n");
       printf("\n Sair                8\n");

       printf("\nDigite uma opcao : \n");
       scanf("%d", &opMenu);
       limparTela();
        switch (opMenu)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
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
            // limparTela();
            // char listarNC;
            // printf("\nListar por nome ou por codigo dos produtos? (n/c)\n");
            // scanf("%c", &listarNC);
            // if(listar == 'n')
            //     LISTAR NOME;
            // else
            // {
            //     LISTAR COD
            // }
            
            break;
         case 8:
            return 0;
            break;
        }
        // limparTela();

    } while (opMenu != 8);  

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
            printf("Erro de alocao");
        }
        
        *alocado += *alocado;
               
    }

    char nome[TAM_NOME];

    printf("Digite o nome do produto: \n\n");
    scanf("%s",&nome );

    strcpy(estoque[*nCadastros].nome, nome);

    printf("Nome Est: %s\n", estoque[*nCadastros].nome);
    printf("Digite a quantidade do produto em estoque: \n");
    scanf("%d", &estoque[*nCadastros].qtd);
    estoque[*nCadastros].codigo = (*id) + 1;    
    
    printf("estoque[*nCadastros].nome: %s\n", estoque[*nCadastros].nome);
    printf("estoque[*nCadastros].qtd: %d\n", estoque[*nCadastros].qtd);
    printf("estoque[*nCadastros].codigo: %d\n", estoque[*nCadastros].codigo);

    *nCadastros ++;
    *id++;

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