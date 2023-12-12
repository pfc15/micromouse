# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

typedef struct no no;

struct no{
    int x;
    int y;
    no* pai;
    no* filhos[4];
    int visitado[4];
};

no* No(int x, int y, no* pai){
    no* novo = malloc(sizeof(no));
    novo->x = x;
    novo->y = y;
    novo->pai = pai;
    if (pai != NULL){
        if (pai->x==x){
            pai->filhos[(y+2)%y] = novo;
            pai->visitado[(y+2)%y] = 1;
        } else{
            pai->filhos[(x+1)%x] = novo;
            pai->visitado[(x+1)%x] = 1;
        }
    }
    return novo;
}


int fazer(const char acao){
    printf("%c\n", acao);
    fflush(stdout);
    int retorno;
    scanf("%d", &retorno);
    return retorno;
}

void pra_frente(no* pai, int x, int y){
    no* filho = No(x, y, pai);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("estou em (%d, %d) e vou andar para frente\n", x, y);
    int retorno = fazer('w');
    if (retorno == 1){
        pra_frente(filho, x+1, y);
        printf(", (%d, %d)", filho->x, filho->y);
    } else {
        printf("voltando:\n");
        printf("(%d, %d)", filho->x, filho->y);
    }
}

void dfs(no* atual, int x_ant, int y_ant){

    int retorno = fazer("w");


    
}

int main(){

    int retorno;
    char acao;
    pra_frente(NULL, 0, 0);
    printf("\n");





    return 0;
}