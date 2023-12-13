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

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
# define Hash(x,y, m) (mod((((x*x)+(3*x)+(2*x*y)+y+(y*y))/2),m))
# define COLint 10




int htfind(no **ht, long int M, no* tf){
    int h = Hash(tf->x, tf->y, M);
    for (int c=0, i=h;c<COLint;i =(i+1)%M, c++){
        if (ht[i]==tf) return i;
    }
    return -1;
}

int htfind_coordenada(no **ht, long int M, int tf_x, int tf_y){
    long int h = Hash(tf_x, tf_y, M);
    for (long int c=0, i=h;c<COLint;i =(i+1)%M, c++){
        if (ht[i]!=NULL)
            if (ht[i]->x == tf_x && ht[i]->y==tf_y) return i;
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


// construtor do nó
no* No(int x, int y, no* pai){
    no* novo = malloc(sizeof(no));
    novo->x = x;
    novo->y = y;
    novo->pai = pai;
    for (int i =0; i<4;i++){
        novo->visitado[i] = 0;
    }
    if (pai != NULL){
        if (pai->x==x){
            pai->filhos[mod((pai->y+2)-y,4)] = novo;
            pai->visitado[mod((pai->y+2)-y,4)] = 1;
            novo->visitado[mod((pai->y+4)-y,4)] = 1;
        } else{
            pai->filhos[mod(pai->x+1-x,4)] = novo;
            pai->visitado[mod(pai->x+1-x,4)] = 1;
            novo->visitado[mod(pai->x+3-x,4)] = 1;
        }
    }
    return novo;
}

// mandar ação para o programa
int fazer(const char acao){
    printf("%c\n", acao);
    fflush(stdout);
    int retorno;
    scanf("%d", &retorno);
    return retorno;
}

// função para verificar se já exploramos tudo do nó
int esgotado(no* analisa){
    int cont =0;
    for (int i=0;i<4;i++){
        printf("v[%d]: %d\n", i, analisa->visitado[i]);
        if (analisa->visitado[i]!=0){
            cont++;
        }
    }
    if (cont == 4) return 0;
    else return -1;
}

// função para retornar ao pai


int pra_frente(no* atual, no **visit, int direcao){
    
    no *prox = NULL;
    int x, y;
    int retorno = -1;
    while (1){
        prox = NULL;
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("estou em (%d, %d) e vou andar para frente\ndirecao: %d\n", atual->x, atual->y, direcao);

        
        if (direcao%2==0){
            x = atual->x+(((direcao-1)%2)*-1);
            y = atual->y;
        } else{
            x = atual->x;
            y = atual->y+(((direcao-2)%2)*-1);
        }

        // verificando se já visitamos tudo
        int index = htfind_coordenada(visit,100000, x, y);
        if (index>=0){
            prox = visit[index];
            if (esgotado(prox)==-1){
                retorno = fazer('w');
            } else{
                atual->visitado[direcao] = 1;
                prox = NULL;
                retorno=0;
            } 
        } else{
            retorno = fazer('w');
        } 


        if (retorno == 1){ // se deu certo
            if (prox==NULL){
                prox = No(x, y, atual);
                HTinsert(visit,100000, prox);
            }
            retorno = pra_frente(prox , visit, direcao);
        }
        
        if (retorno==0){ // se bateu na parede
            atual->visitado[direcao] = -1;
            int parar = esgotado(atual);
            while (parar==-1){
                direcao = (direcao+1)%4;
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("0:x+1  1:y+1  2:x-1  3:y-1\n(%d, %d) direção: %d\n", atual->x, atual->y, direcao);
                fazer('l');
                if (atual->visitado[direcao]==0) break; // se cont >4 vai ter q implementar voltar para ultimo lugar
            }
            if (parar==0) break;
        } else if (retorno==2){ // se achou o final
            printf("(%d, %d)", atual->x, atual->y);
            return 2;
        } else if (retorno ==3){
            int c =0;
            if (esgotado(atual)== 0) return 3;
            break;
        }
    }
    return 0;
}
    


int main(){
    no** visitados = HTinit(100000);
    int retorno;
    char acao;
    no* cabeca = No(0,0,NULL);
    pra_frente(cabeca, visitados, 0);
    printf("\n");





    return 0;
}