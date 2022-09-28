
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>




int main(int argc, uint8_t **argv) {
int delta[] = {3, 1<<16, 32, 2},pn,poffset;
uint8_t *pc;

int PINTERP_LEN = 100;
char *op[] = {"<xC#;:9\3708765\3154321\2420/.-w,+*)('&%$<L",
"\353\326\267\223\276\260\214o\270\232\305\261h\215\224\277\262D\271i\216\263 \360Ej\217K\370\264p\225v\272\233\300\241\306\314\220kF\226!lq\234G\"HLrw\242#$MxN\374\357\375\376\377'\367\000()R*ST}~\2500/Z\006\005.6Y\204\f\004\v5`\022\003<\n\021\030-\371\0364;XB_f\203\212\256",
"\025*\330\322\264\256\250\217\220\255\212\204~\247\242kl\211f`\203ZT}xde\202_YS|w^XRFG#HB<60!*NWQ@A;5/)M9:4.(23-'\033\031,&",
"\243F\330\331\375\374\376\"! #GFED\003H\260lkjih('MLK.NrqpoSRxwvY~}\204\250",
"\2300\256\232\264\272\300\017\t4\352\3443\003.Y\016\336\b-X2\330W\002,V\322\233\374&P\243z{Q|'R}\375(S~tJ u\366nKvD\032ho!Lw>bEpi",
};
  char _[sizeof(struct winsize)];
  unsigned short*__=_;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, __);
  int u = __[1] -2;
  int q1 = __[0] - 2;
  int **data = malloc(sizeof(int *) * u);
  for (int i = 0; i < u; i++) {
    data[i] = malloc(sizeof(int) * q1);
    memset(data[i], 1, sizeof(int) * q1);
    for (int j = 0; j < q1; j++)
      data[i][j] = rand() % delta[0] != 0;
  }
  int cell_rows=q1, cell_cols=u;

  int TIMING=100000,*args[] = {&delta[0], &delta[1], &delta[2], &PINTERP_LEN, &TIMING, &delta[3]},idx,new[cell_cols][cell_rows],i,j,v,w,d,p,sum;
  long indices[] = {0x1b0048303b305b1b, 0x64253b353b38345b, 0x206d, 0x401040};
  for (int i = 1; i < argc; i++) {
    int val = atoi(argv[i]);
    if (val < 0) continue;
    *args[i - 1] = val;
  }

  poffset = (*(uint8_t *) &op[delta[3]][0]);
  pn = (int) ((*(unsigned char *) &op[delta[3]][1]) - poffset) & 0xFF;
  pn += 2;
  pc = &op[delta[3]][0];
  pc = malloc(pn*2);
  for (i=0;i<pn;i++) {
    uint8_t c = op[delta[3]][i];
    pc[i]= (c-poffset)&0xFF;
  }
  srand(time(0));
  char *x = "";
start:
  i=1;
i:
    j = 1;
    j:
    {
    sum = 0;
    for (v=0; v < 9;v++) sum += data[i + 1-v/3][j + 1-v%3] == 0;
    d = data[i][j];
    int k[] = {0, d > delta[2] ? 0 : rand() % delta[1] == 0 ? 0 : d + 1};
    p = 0;
    switch (sum) {
      case 2:if (d == 0) break;
      default:p=1;case 3:;
    }
    new[i][j] = k[p];
    j++;
    if (j < cell_rows - 1) goto j;
    }
    i++;
    if (i < cell_cols - 1) goto i;

  for (int i = 1; i < cell_cols - 1; i++)
    memcpy(&data[i][1], &new[i][1], 4 * (cell_rows - 2));

  int buffer[8 * cell_rows * cell_cols];
  uint8_t *pos = &buffer[0];
  fputs((uint8_t*)indices,stdout);
  for (int j = 0; j < cell_rows; j++) {
    for (int i = 0; i < cell_cols; i++) {
      idx = (int)(pn*(float) data[i][j] / (float) PINTERP_LEN);
      idx = idx >= pn ? pn - 1 : idx;
      pos += sprintf(pos, 7+(uint8_t*)indices, ((uint8_t *) pc)[idx]);
    }
    pos[0] = '\n';
    pos++;
  }
  pos[0] = 0;
  puts(buffer);
  fflush(0);
  usleep(TIMING);
  goto start;
}

