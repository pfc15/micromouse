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

# define Hash(x,y, m) ((((x*x)+(3*x)+(2*x*y)+y+(y*y))/2)%m)
# define COLint 10

int htfind(no **ht, long int M, no* tf){
    int h = Hash(tf->x, tf->y, M);
    for (int c=0, i=h;c<COLint;i =(i+1)%M, c++){
        if (ht[i]==tf) return i;
    }
    return -1;
}

int htfind_coordenada(no **ht, long int M, int tf_x, int tf_y){
    int h = Hash(tf_x, tf_y, M);
    for (int c=0, i=h;c<COLint;i =(i+1)%M, c++){
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
            pai->filhos[((pai->y+2)%y)%4] = novo;
            pai->visitado[((pai->y+2)%y)%4] = 1;
            novo->visitado[((pai->y+4)%y)%4] = 1;
        } else{
            pai->filhos[((pai->x+1)-x)%4] = novo;
            pai->visitado[((pai->x+1)-x)%4] = 1;
            novo->visitado[((pai->x+3)-x)%4] = 1;
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

void pra_frente(no* pai, int x, int y, no **visit, int direcao){

    int visitado = htfind_coordenada(visit, 100000, x, y);
    no* filho;
    if (visitado>=0){
        printf("opa, já vim aqui!\n");
        
        filho = visit[visitado];
        printf("(%d, %d)\n", filho->x, filho->y);
        fflush(stdout);
    }
        
    else{
        filho = No(x, y, pai);
        HTinsert(visit, 100000, filho);
    }
        
    
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("estou em (%d, %d) e vou andar para frente\ndirecao: %d\n", x, y, direcao);

    int retorno = -1;
    while (retorno!= 1){
        int retorno = fazer('w');
        if (retorno == 1){ // se deu certo
            if (direcao%2==0){
                pra_frente(filho, x+((direcao-1)*-1), y, visit, direcao);
            } else{
                pra_frente(filho, x, y+((direcao-2)*-1), visit, direcao);
            }
            printf(", (%d, %d)", filho->x, filho->y);
        }
        
        if (retorno==0){ // se bateu na parede
            filho->visitado[direcao] = -1;
            int cont =0;
            while (1){
                direcao = (direcao+1)%4;
                cont++;
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("0:x+1  1:y+1  2:x-1  3:y-1\ndireção: %d\n", direcao);
                for (int i=0;i<4;i++) printf("v[%d]: %d\n", i, filho->visitado[i]);
                fazer('l');
                if (filho->visitado[direcao]==0 || cont>4) break;
                
            }
            
        } else if (retorno==2){ // se achou o final
            printf("voltando:\n");
            no* atual = visit[htfind(visit, 100000, filho)];
            printf("(%d, %d) %d", filho->x, filho->y, atual == filho);
            break;
        } else break;
    }
    
    
}
    
    


int main(){
    no** visitados = HTinit(100000);
    int retorno;
    char acao;
    pra_frente(NULL, 0, 0, visitados, 0);
    printf("\n");





    return 0;
}