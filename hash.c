#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HASH(X, Y, M) (((X)*(X)+3*(X)+2*(X)*(Y)+(Y)+(Y)*(Y)/2) % (M))
#define COLint 10

typedef struct {
    int x, y;
    no* pai;
    no* filhos[4];
    int visitado[4];
} no;

int HTfind(no** ht, long int M, no* tf) {
    int h = HASH(tf->x, tf->y, M);
    int c;
    for (c = 0, i = h; c < COLint; i = (i+1) % M, c++)
        if (ht[i] == tf)
            return i;
    return -1;
}

int HTinsert(no** ht, long int M, no* novo) {
    int h = HASH(novo->x, novo->y, M);
    int f = HTfind(ht, M, novo);
    if (f >= 0)
        return -1;
    int c;
    for(c = 0; ht[h] != -1 && c < COLint; c++)    // Obs.: quando o laço terminar, c pode ter um valor no intervalo [0, 10] (pois COLlint vale 10) 
        h = (h + 1) % M;
    if (c > COLint)                        // será que não é c >= COLint? 
        return -1;
    ht[h] = novo;
    return 1;
}

no** HTinit(long int M) {
    no **nht = malloc(M * sizeof(no*));
    for (int i = 0; i < M; i++)            // SUGESTÃO: memset(nht, NULL, M * sizeof(no*));
        nht[i] = NULL;
    return nht;
}
