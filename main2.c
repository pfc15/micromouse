#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Vec2 Vec2;
typedef struct Square Square;
typedef struct Mouse Mouse;
typedef struct Queue Queue;
typedef struct SymTable SymTable;
typedef struct Node Node;
typedef struct Aproxim Aproxim;
enum directions { NORTH = 0, WEST, SOUTH, EAST, FRONT, LEFT, BACK, RIGHT };
enum search_status {NOT_FOUND, APROACHED, FOUND = 2};
enum estimated_axes {NONE, X, Y, BOTH};

SymTable  STinit(const unsigned);
Square  QUEUEdeltop(Queue *q);
Queue QUEUEinit(const size_t size);
Vec2  STfind(const Vec2);
void  STinsert(const Vec2);
void  QUEUEinsert(Queue* q, const Square item);
bool  QUEUEempty(const Queue q);
bool  node_seen(Vec2);
void  fixUp(Queue *q, const size_t i) ;
void  fixDown(Queue* q, const size_t i);
bool  nearer(const Vec2 v1, const Vec2 v2);
int   abs_dist(const Vec2, const Vec2);
void  node_root(const Vec2);
void  node_mark(Vec2);
void  node_connect(Vec2, Vec2);
void  node_children(Vec2, Vec2) ;
void  mouse_move_to_exit();
Vec2  neighbor(const Vec2, const int);
Vec2  here();
int   walk();

Node* tree;
Queue pq;
SymTable ht;
Aproxim aprox;

const unsigned MIN_DIST = 10;

Mouse mouse;        // inicializado com {{0, 0}, {0, 0}, NORTH}
int main() 
{
    tree = (Node*) malloc(10000 * sizeof(Node));
    pq = QUEUEinit(10000);
    ht = STinit(100000);

    tree[0].dad = 0;        // inicializa o 1º nó
    tree[0].depth = 0;      //

    aprox.status = NOT_FOUND;
    aprox.pos = (Vec2) {0, 0};

    while(aprox.status != FOUND) // enquando a saída não for encontrada
    {
        if(aprox.status != APROACHED)   // se a última estimativa feita não é boa o suficiente
        {
            aprox.axes = NONE;              // atualiza a aproximação
            aprox.pos_prev = aprox.pos;     //
            aprox.man_prev = aprox.man;     //
            aprox.status = estimate();      ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        mouse_move_to_exit();
    }
    
    return 0;
}

struct Vec2     { int x; int y; };
struct Square   { Vec2 pos; unsigned index; bool visited;};
struct Node     { unsigned dad, depth, son[4]; };
struct Mouse    { Vec2 pos, prev_pos; unsigned dir; };
struct Queue    { Square *queue; unsigned size, tail; };
struct SymTable { Square *st; unsigned size;};
struct Aproxim  { Vec2 pos, pos_prev; unsigned man, man_prev, status, axes; };

unsigned estimate()
{
    // visita um vizinho
}
void mouse_move_to_exit()
{
    // deve atualizar status (NOT_FOUND => APROACHED, APROACHED => NOT_FOUND, APROACHED => FOUND)
    
}
bool nearer(Vec2 v1, Vec2 v2)
{
    return abs_dist(v1, estim_exit) < abs_dist(v2, estim_exit);
}
void fixUp(Queue *q, size_t i) 
{
    Square* v = q->queue;
    int son = i;
    while(son > 1 && nearer(v[son].pos, v[son/2].pos)) {     // se filho for mais prioritário que seu pai
        Square t = v[son];
        v[son] = v[son/2];
        v[son/2] = v[son];
        son /= 2;
    }
}
void fixDown(Queue* q, size_t i)
{
    Square* v = q->queue;
    int son, dad = i;
    while((son = 2*dad) <= q->tail) {
        if(son < q->tail && nearer(v[son+1].pos, v[son].pos))  // son deve ser o filho mais prioritário
            son++;
        if(!nearer(v[son].pos, v[dad].pos))       // se dad não for menos prioritário que son, fim
            break;
        Square t = v[son];                // rebaixa dad
        v[son] = v[dad];
        v[dad] = v[son];
        dad = son;
    }
}
Queue QUEUEinit(size_t size)
{
    Queue q;
    q.tail = 0;
    q.size = size;
    q.queue = (Square*) malloc((size+1) * sizeof(Square));
    return q;
}
bool QUEUEempty(Queue q)
{
    return q.tail <= 0;
}
Square QUEUEdeltop(Queue *q)
{
    if(QUEUEempty(*q))
        perror("ERRO: tentou deletar topo de fila vazia\n"), exit(-8);
    Square item = q->queue[1];
    q->queue[1] = q->queue[(q->tail)--];
    fixDown(q->queue, 1);
    return item;
}
void QUEUEinsert(Queue* q, Square item)
{
    if(q->tail >= q->size)
        q->queue = realloc(q->queue, 2*(q->size));
    q->queue[++(q->tail)] = item;
    fixUp(q->queue, q->tail);
}
int abs_dist(Vec2 v1, Vec2 v2) {
    return abs(v1.x-v2.x)+abs(v1.y-v2.y);
}

// REVISAR:
void turn(int dir)
{
    char cmd = 'l';     // comando (left por padrão)
    int reps;           // nº rep. do comando

    if(dir>=NORTH && dir<=EAST) /*se a direção é norte, sul, leste ou oeste*/
        reps = (dir - mouse.dir + 4) % 4;
    else if(dir>=FRONT && dir<=RIGHT) /* se a direção é frente, trás, esquerda e direita*/
        reps = dir % 4;
    else
        /*ERRO*/; 

    if(reps == 3)   cmd = 'r', reps = 1;          // 3 * left == right 
    for(int i = 0; i < reps; ++i)
    {
        fflush(stdout);
        printf("%c\n", cmd);    // dá o comando
        scanf("%*d");           // ignora o retorno
    }
}
void node_schedule() // tenta agendar quadrado à frente. Ao final, Mouse vai estar de costas para esse quadrado.
{
    Vec2 son = neighbor(mouse.pos, mouse.dir);
    if (already_seen(son))
        return; // se o quadrado à frente já é conhecido
    Vec2 dad = mouse.pos;
    if (walk())
    {                       // Se andou:
        add_son(son, dad);  // 	    insere son na árvore e na agenda.
        turn(BACK);         //	    dá meia volta,
        walk();             //	    volta para o quadrado pai
    }
    else
    {                       // Se bateu:
        turn(BACK);         //	    dá meia volta.
    }
}
int estimate()
{
    while((estim_axes != BOTH) && exit_status != FOUND)
    {
            if(estim_axes == NONE) {
                // walk in any direction
                if(estim_axes == X)
                else
            }
            else
    }
}
// REVISADOS:
int walk() // VERIFIQUE SE CHEGOU NA SAÍDA!
{
    fflush(stdout);
    printf("w\n");
    int b;
    scanf("%d", &b);
    return b;
};
int man()
{
    int d;
    fflush(stdout);
    printf("d\n");
    scanf("%d", &d);
    if(d < 0) exit(-3);
    return d;
}
Vec2 neighbor(const Vec2 pos, const int abs_dir)
{
    Vec2 t = pos;
    switch(abs_dir)
    {
        case NORTH: t.y++;
        break;
        case SOUTH: t.y--;
        break;
        case EAST:  t.x++;
        break;
        case WEST:  t.x--;
        break;
        default:    exit(-3);
        break;
    }
    return t;
}

/*
1. tire o manhattan 0.
2. tente andar em uma direção, guarde a casa em que estava e a direção que andou.
3. tire o manhattan 1 e estime a saída
4. continue andando reto até bater
5. tente andar numa direção ortogonal à anterior:
6.      se não conseguir, então regrida uma casa. volte ao passo 3.
7.      se conseguir, tire o manhattan 2 e estime da sída.
// agora, a saída está estimada nas duas direções x e y.
8. ande na direção da saída estimada até chegar a certa distância
*/
