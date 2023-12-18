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

typedef struct no_volta no_volta;

struct no_volta{
    int x;
    int y;
    no_volta* pai;
    char comando[5];
};
no_volta* volta = NULL;
no_volta* novo;

# define Hash(x,y, m) (mod((((x*x)+(3*x)+(2*x*y)+y+(y*y))/2),m))
# define COLint 10

long int m;



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
        h =((h+1)%M);
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
    if (analisa == NULL) return 0;
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
int retornar(no* pai, no* filho, int direcao, const int gravar){
    int direcao_desejada; int cont = 0;
    if (pai!=NULL){
        if (pai->x==filho->x){
            direcao_desejada = mod((pai->y-filho->y)+4, 4);
            while(direcao!=direcao_desejada){
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("0:x+1  1:y+1  2:x-1  3:y-1\nfilho: (%d, %d); pai: (%d, %d) direção: %d\n", filho->x, filho->y, pai->x, pai->y, direcao);
                direcao = (direcao+1)%4;
                fazer('l');
                if (gravar){
                    novo->comando[cont] = 'r';
                    cont++;
                }
            }
        } else{
            direcao_desejada = mod((pai->x-filho->x)+3, 4);
            while(direcao!=direcao_desejada){
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("0:x+1  1:y+1  2:x-1  3:y-1\nfilho: (%d, %d); pai: (%d, %d) direção: %d\n", filho->x, filho->y, pai->x, pai->y, direcao);
                
                direcao = (direcao+1)%4;
                fazer('l');
                if (gravar){
                    novo->comando[cont] = 'r';
                    cont++;
                }
            }
        }
        if (gravar){
            novo->comando[cont] = 'w';
            novo->comando[cont+1] = '\0';
        } 
            
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("estou em (%d, %d) e vou andar para frente\ndirecao: %d\n", filho->x, filho->y, direcao);
        fazer('w');
    } 
    
    return direcao;
}



int pra_frente(no* ant, no* atual, no **visit, int direcao){
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
        int index = htfind_coordenada(visit,m, x, y);
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
            if (retorno==2){
                printf("------------------------------------------------------------------\n");
                printf("CHEGAMOS! AGR VAMOS VOLTAR!!\n");
                fflush(stdout);
                if (prox==NULL){
                    prox = No(x, y, atual);
                    HTinsert(visit,m, prox);
                }
                retornar(atual, prox, direcao, 0);
            }
        } 
        if (retorno == 0){
            atual->visitado[direcao] = -1;
        }
        

        if (retorno == 1){ // se deu certo
            if (prox==NULL){
                prox = No(x, y, atual);
                HTinsert(visit,m, prox);
            } else{
                atual->visitado[direcao] = 1;
                prox->visitado[mod(direcao-2, 4)] = 1;
            }
            retorno = pra_frente(atual, prox , visit, direcao);
            if (retorno == 3 || retorno == 0){
                direcao = mod(direcao-2, 4);
            }
        }
        
        if (retorno==0){ // se bateu na parede
            
            int parar = esgotado(atual);
            while (parar==-1){
                if (atual->visitado[direcao]==0) break; 
                direcao = (direcao+1)%4;
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("0:x+1  1:y+1  2:x-1  3:y-1\n(%d, %d) direção: %d\n", atual->x, atual->y, direcao);
                fazer('l');
            }
            if (parar==0) break;
        } else if (retorno==2){ // se achou o final
            direcao = mod(direcao-2, 4);
            novo = malloc(sizeof(no_volta));
            novo->x = atual->x; novo->y = atual->y;
            retornar(atual->pai, atual, direcao, 1);
            novo->pai = volta;
            volta = novo;
            printf("(%d, %d)\n", atual->x, atual->y);
            return 2;
        } else if (retorno ==3){
            direcao = mod(direcao-2, 4);
            if (esgotado(ant) == 0){
                retornar(atual->pai, atual, direcao, 0);
            } else{
                retornar(ant, atual, direcao, 0);
            }
            
            if (esgotado(atual)== 0){
                return 3;
            } 
            break;
        }
    }
    retornar(ant, atual, direcao, 0);
    return 0;
}


void caminho_rapido(no_volta* n){
    no_volta*aux; int cont; int corrida;
    while (n->pai!=NULL){
        cont = 0;corrida = 0;
        if (n->comando[0] == n->comando[1] && n->comando[1] == n->comando[2]){
            if (n->comando[0] == 'l'){
                strcpy(n->comando, "rl\0");
            } else{
                strcpy(n->comando, "lw\0");
            }
        }
        while(n->comando[cont]!='\0'){
            switch (n->comando[cont])
            {
            case 'l':
                fazer('l');
                break;
            case 'r':
                fazer('r');
                break;
            case 'w':
                aux = n->pai;
                corrida = 1;
                while (aux->comando[0]=='w' && corrida<=4){
                    corrida++;
                    aux= aux->pai;
                }
                break;

            default:
                break;
            }
            cont++;
        }
        switch (corrida){
            case 1:
                fazer('w');
                break;
            case 2:
                fazer('j');
                break;
            case 3:
                fazer('R');
                break;
            case 4:
                fazer('s');
                break;
        }
        n = aux;
    }
}


int main(){
    m=3;
    for (int i=0;i<15;i++) m = m *3;
    no** visitados = HTinit(m);
    volta = malloc(sizeof(no_volta));
    volta->x = 0; volta->y = 0;
    volta->pai = NULL;
    
    
    int retorno;
    char acao;
    no* cabeca = No(0, 0,NULL);

    HTinsert(visitados,m,cabeca);
    pra_frente(NULL, cabeca, visitados, 0);
    strcpy(volta->comando, "llw\0");
    printf("----------------------------------\n");
    printf("CAMINHO COMPLETO\n");
    printf("----------------------------------\n");
    caminho_rapido(volta);
    //while(volta != NULL){
    //    printf("(%d, %d) comando: %s\n ", volta->x, volta->y, volta->comando);
    //    volta = volta->pai;
    //}
    printf("\n");





    return 0;
}