#include <stdio.h>

#define p putchar
#define m malloc

#define idx(row, col) (row * ncols) + col

void display(int nrows, int ncols, char* best, char c) {
    printf("\033[%d;%dH", 1, 1);
    for (int i = 0; i < nrows * ncols; i += 1) {
        if ((c=best[i]&&isprint(c)));
        else c=' ';
        putchar(best[i]);
        //p(c);
        if(i%ncols==(ncols - 1)) p('\n');
    }
    /*    for (int col = 0; col < ncols; col += 1) {
            if ((c=target[idx(row, col)])&&isprint(c));
            else c=' ';
            p(c);
        }
        p('\n');
    }*/
}

#define INC_A 0
#define DEC_A 1
#define ZERO_A 2
#define ZERO_B 3

char charset[256];

char random_char() {
    char c;
start:;
    while (!isprint(c=rand()));
    if (!charset[c]) goto start;
    return c;
}

typedef struct __gene {
    int a, b, action;
} Gene;

Gene make_gene(int size) {
    Gene g;
    g.a=rand()%size;
    g.b=rand()%size;
    g.action=random_char();
    return g;
}

typedef struct __genome {
    int nchromosomes, chromosome_len;
    double fitness;
    Gene* genes;
    char* map;
} Genome;

void crossover_gene(Gene* dst, Gene* p1, Gene* p2, int chromosome_len) {
    for (int i = 0; i < chromosome_len; i += 1)
        dst[i] = i < chromosome_len / 2 ? p1[i] : p2[i];
}
void gncpy(Genome* dst, Genome* src) {   
    for (int c = 0; c < dst->nchromosomes*dst->chromosome_len; c++)
        dst[c]=src[c];
}

void mutate(Genome* genome, int nrows, int ncols) { 
    int gene = rand() % (genome->nchromosomes * genome->chromosome_len);
    genome->genes[gene] = make_gene(nrows * ncols);
}

void crossover(Genome* child_dst, Genome** candidate_parents, int n_candidates) {
    int i = rand() % n_candidates;
    int j = (i + 1) % n_candidates, cl = child_dst->chromosome_len;
    Gene* dst=child_dst->genes, *p1=candidate_parents[i]->genes, *p2=candidate_parents[j]->genes; 
    
    // Split each chromosome in half...
    for (int c = 0; c < child_dst->nchromosomes; c++)
        crossover_gene(&dst[c*cl], &p1[c*cl], &p2[c*cl], child_dst->chromosome_len);
}

Genome* make_genome(int nrows, int ncols) {
    Genome* g = m(sizeof(Genome));
    g->nchromosomes = 1;
    g->chromosome_len = nrows * ncols;
    int ngenes = g->nchromosomes * g->chromosome_len;
    g->genes = m(sizeof(Gene)*ngenes);
    for (int i = 0; i < ngenes; i++) {
        g->genes[i] = make_gene(nrows * ncols);
    }
    return g;
}

double fitness(char* target, char* y, int nrows, int ncols) {
    // Mean squared error
    double mse = 0.0;
    for (int i = 0; i < nrows * ncols; i++) {
        if (isprint(target[i])) {
x:;
            double x = (double) (target[i] != y[i]);
            if (target[i] == ' ')
                x *= 10;
            mse += x;
	        continue;
        } else
            target[i] = ' ';
        goto x;

    }
    /*
    for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++) {
            x:;
            if (isprint(target[row][col])) {
                double x = (double) (target[row][col] != y[row][col]);
                if (target[row][col] == ' ')
                    x *= 10;
                mse += x;
	            continue;
            } else
                target[row][col] = ' ';
            goto x;
        }
    */
    return mse;
}

void zmap(char* target, int nrows, int ncols) {
    memset(target, 0, ncols * nrows); 
}

char* make_map(int nrows, int ncols) {
    char* target = malloc(ncols * nrows);
    zmap(target, nrows, ncols);
    return target;
}


void ex(Genome* genome, int nrows, int ncols) {
    char* map = genome->map;
    for (int gene = 0; gene < genome->nchromosomes*genome->chromosome_len; gene++){
        Gene* g = &genome->genes[gene];
        map[g->a] = map[g->b];
        map[g->b] = g->action;
    }
}

int compFitness(const void *p, const void *q) {
    double pp = (*(Genome**) p)->fitness;
    double qq = (*(Genome**) q)->fitness;
    return pp > qq ? 1 : -1;
}

#define POPSIZE 128
#define PARENT_FRAC 8
#define MAX_LINES 128
#define MAX_LINE_LEN 512

int max(int a, int b) { return a > b ? a : b; }

int main(int argn, char** argv) {
    memset(charset, 0, 256);
    
    char** targets = malloc(MAX_LINES * sizeof(char*));
    
    int max_len = -1;
    int ncols = 0;
    int nrows = 0;
    while (!feof(stdin)) {
        targets[nrows] = malloc(MAX_LINE_LEN);
        fgets(targets[nrows], MAX_LINE_LEN, stdin);
        printf("%s", targets[nrows]);
        ncols=max(strlen(targets[nrows]) - 1, ncols);
        for (int j = 0; j < strlen(targets[nrows]); j += 1)
            charset[targets[nrows][j]] = 1;
        nrows++;
    }
    printf("nrows = %d; ncols = %d\n", nrows, ncols);
    char* target = make_map(nrows, ncols);
    
    for (int row = 0; row < nrows; row++) {
        memcpy(&target[idx(row, 0)], targets[row], ncols);
    }

    Genome* population[POPSIZE];
    char* maps[POPSIZE];
    for (int i = 0; i < POPSIZE; i += 1) {
        population[i] = make_genome(nrows, ncols);
        population[i]->map = make_map(nrows, ncols);
    }
    
    system("clear");

    int iter = 0;
    while (1){
        for (int i = 0; i < POPSIZE; i += 1) {
            zmap(population[i]->map, nrows, ncols);
            ex(population[i], nrows, ncols);
            population[i]->fitness = fitness(target, population[i]->map, nrows, ncols);
        }
        qsort(population, POPSIZE, sizeof(Genome*), compFitness);
        for (int i = 0; i < POPSIZE; i += 1)
            if (i >= POPSIZE / PARENT_FRAC) {
                if (rand() & 1)
                    crossover(population[i], population, POPSIZE / PARENT_FRAC);
                else {
                    int parent_idx = rand() % (POPSIZE/PARENT_FRAC);
                    //gncpy(population[i], population[parent_idx]);
                    mutate(population[i], nrows, ncols);
                }
            } else {
                mutate(population[i], nrows, ncols);
            }

        if (iter++ % 10 == 0) {
            char*best=population[0]->map,c;
            printf("\033[%d;%dH", 1, 1);
            for (int i = 0; i < nrows * ncols; i += 1) {
                if ((c=best[i]&&isprint(c)));
                else c=' ';
                putchar(best[i]);
                //p(c);
                if(i%ncols==(ncols - 1)) p('\n');
            }

            // display(nrows, ncols, population[0]->map, nrows * ncols);
            printf("\n\nBest fitness: %f; iter %d \t\t\t\n", population[0]->fitness, iter);
        }
        //printf("%s\n", population[0]->map);
    }

}
