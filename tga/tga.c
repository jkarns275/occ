#include <stdio.h>

#define p putchar
#define m malloc

void display(int nrows, int ncols, char** target, char c) {
    printf("\033[%d;%dH", 0, 0);
    for (int row = 0; row < nrows; row += 1) {
        for (int col = 0; col < ncols; col += 1) {
            if ((c=target[row][col]) && c != '\n');
            else c=' ';
            p(c);
        }
        p('\n');
    }
}

#define INC_A 0
#define DEC_A 1
#define ZERO_A 2
#define ZERO_B 3

typedef struct __gene {
    int a[2];
    int b[2];
    int action;
} Gene;

Gene make_gene(int nrows, int ncols) {
    Gene g; // m(sizeof(Gene));
    g.a[0]=rand()%nrows;
    g.a[1]=rand()%ncols;
    g.b[0]=rand()%nrows;
    g.b[1]=rand()%ncols;
    g.action=rand() & 0xFF;
    return g;
}

typedef struct __genome {
    int nchromosomes, chromosome_len;
    double fitness;
    Gene** genes;
    char** map;
} Genome;

void crossover_gene(Gene* dst, Gene* p1, Gene* p2, int chromosome_len) {
    for (int i = 0; i < chromosome_len; i += 1)
        dst[i] = i < chromosome_len / 2 ? p1[i] : p2[i];
}
#define NMUTATIONS 14
void mutate(Genome* genome, int nrows, int ncols) {
    for (int i = 0; i < NMUTATIONS; i += 1) {
        int chr = rand() % genome->nchromosomes;
        int gen = rand() % genome->chromosome_len;
        genome->genes[chr][gen] = make_gene(nrows, ncols);
    }
}

void crossover(Genome* child_dst, Genome** candidate_parents, int n_candidates) {
    int i = rand() % n_candidates;
    int j = (i + 1) % n_candidates;
    // Split each chromosome in half...
    for (int c = 0; c < child_dst->nchromosomes; c++)
        crossover_gene(child_dst->genes[c], candidate_parents[i]->genes[c], candidate_parents[i]->genes[c], child_dst->chromosome_len);
}



Genome* make_genome(int nrows, int ncols) {
    Genome* g = m(sizeof(Genome));
    g->nchromosomes = nrows;
    g->chromosome_len = nrows * ncols;
    g->genes = m(sizeof(Gene*)*g->nchromosomes);
    for (int i = 0; i < g->nchromosomes; i++) {
        g->genes[i] = m(sizeof(Gene)*g->chromosome_len);
        for (int j = 0; j < g->chromosome_len; j++) {
            g->genes[i][j] = make_gene(nrows, ncols);
        }
    }
    return g;
}

double fitness(char** target, char** y, int nrows, int ncols) {
    // Mean squared error
    double divisor = (double) nrows * ncols;
    double mse = 0.0;
    for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++) {
            double x = ((double)target[row][col]) - ((double)y[row][col]);
            x *= x;
            mse += (x / divisor);
        }
    return mse;
}

void zmap(char** target, int nrows, int ncols) {
    for (int i = 0; i < nrows; i++) {
        memset(target[i], 0, ncols + 2);
    }
}

char** make_map(int nrows, int ncols) {
    char**target = malloc(sizeof(char*) * nrows);
    for (int i = 0; i < nrows; i++)
        target[i] = malloc(ncols + 2);
    zmap(target, nrows, ncols);
    return target;
}


char** ex(Genome* genome, int nrows, int ncols) {
    char** map = genome->map;
    for (int ch = 0; ch < genome->nchromosomes; ch++){
        for (int ge = 0; ge < genome->chromosome_len; ge++) {
            Gene* g = &genome->genes[ch][ge];
            int *t = &g->a[0],tm[4];
            map[t[0]][t[1]] = g->action;
            
            tm[0] = map[t[0]][t[1]];
            map[t[0]][t[1]]=map[t[2]][t[3]];
            map[t[2]][t[3]] = tm[0];
        }
    }
    return map;
}

int compFitness(const void *p, const void *q) {
    double pp = (*(Genome**) p)->fitness;
    double qq = (*(Genome**) q)->fitness;
    return pp > qq ? 1 : -1;
}

int map_sum(char** c, int nrows, int ncols) {
    int sum = 0;
    for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++)
            sum += (int) c[row][col];
    return sum;
}

#define POPSIZE 16

int main(int argn, char** argv) {
    int nrows = atoi(argv[1]);
    int ncols = atoi(argv[2]);

    char* target[nrows];
    for (int i = 0; i < nrows; i += 1) {
        char* line = malloc(ncols + 2);
        memset(line, 0, ncols + 2);
        fgets(line, ncols + 2, stdin);
        printf("%s", line);
        target[i] = line;
    }

    Genome** population = m(sizeof(Genome*) * POPSIZE);
    char*** maps = m(sizeof(char**) * POPSIZE);
    for (int i = 0; i < POPSIZE; i += 1) {
        population[i] = make_genome(nrows, ncols);
        population[i]->map = make_map(nrows, ncols);
    }

    int iter = 0;
    while (1){
        for (int i = 0; i < POPSIZE; i += 1) {
            zmap(population[i]->map, nrows, ncols);
            ex(population[i], nrows, ncols);
            population[i]->fitness = fitness(target, population[i]->map, nrows, ncols);
        }
        qsort(population, POPSIZE, sizeof(Genome*), compFitness);
        for (int i = POPSIZE / 2; i < POPSIZE; i += 1)
            crossover(population[i], population, POPSIZE / 2);
        for (int i = 0; i < POPSIZE; i += 1) {
            mutate(population[i], nrows, ncols);
        }
        
        //printf("best map sum: %d\n", map_sum(population[0]->map, nrows, ncols));
        //printf("Best fitness: %f; iter %d\n", population[0]->fitness, iter++);
        display(nrows, ncols, population[0]->map, nrows * ncols);
    }

}
