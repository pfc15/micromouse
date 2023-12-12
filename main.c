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

# define Hash(x,y, m) (((x*x)+(3*x)+(2*x*y)+y+(y*y)/2)%m)
# define COLint 10

int htfind(no **ht, long int M, no* tf){
    int h = Hash(tf->x, tf->y, M);
    for (int c=0, i=h;c<COLint;i =(i+1)%M, c++){
        if (ht[i]==tf) return i;
    }
    return -1;
}

int HTinsert(no** ht, long int M, no* novo){
    int c=0;
    int h = Hash(novo->x, novo->y, M);
    int f = htfind(ht, M, novo);
    if (f>=0) return -1;
    while(ht[h]!=NULL&&c<COLint){
        h =(h+1%M);
        c++;
    }
    if (c>COLint) return -1;
    ht[h] = novo;
    return 1;
}

no **HTinit(long int M){
    no **nht = malloc(sizeof(no*)*M);
    for (int i=0;i<M;i++){
        nht[i] = NULL;
    }
    return nht;
}



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

void pra_frente(no* pai, int x, int y, no **visit){
    no* filho = No(x, y, pai);
    HTinsert(visit, 100000, filho);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("estou em (%d, %d) e vou andar para frente\n", x, y);
    int retorno = fazer('w');
    if (retorno == 1){
        pra_frente(filho, x+1, y, visit);
        printf(", (%d, %d)", filho->x, filho->y);
    } else {
        printf("voltando:\n");
        no* atual = visit[htfind(visit, 100000, filho)];
        printf("(%d, %d)", atual->x, atual->y);
    }
}


int main(){
    no** visitados = HTinit(100000);
    int retorno;
    char acao;
    pra_frente(NULL, 0, 0, visitados);
    printf("\n");





    return 0;
}