# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>

# define Hash(k, m) (k%m)
#define COLint 10

int htfind(long int *ht, long int M, long int tf){
    int h = Hash(tf, M);
    for (int c=0, i=h;c<COLint;i =(i+1)%M, c++){
        if (ht[i]==tf) return i;
    }
    return -1;
}

int HTinsert(long int* ht, long int M,long int novo){
    int c=0;
    int h = Hash(novo, M);
    int f = htfind(ht, M, novo);
    if (f>=0) return -1;
    while(ht[h]!=-1&&c<COLint){
        h =(h+1%M);
        c++;
    }
    if (c>COLint) return -1;
    ht[h] = novo;
    return 1;
}

long int *HTinit(long int M){
    long int *nht = malloc(sizeof(long int)*M);
    for (int i=0;i<M;i++){
        nht[i] = -1;
    }
    return nht;
}




