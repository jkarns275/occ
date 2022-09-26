#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
typedef uint8_t c;
long long op[][32] = {
  { -72085051763851232, -535372024315593475, -1012478735179974921, -1518859942647304133, 268495081 },
  { 6438518218540711978, -6727352546593177765, -4123106167991476063, -6085585024797342267, -3110386793744314673 },
  { -7376969019393310653, -7896657920757368173, 5003019049271514765, 4920865734552414271, 3616739334958507838, 1516617003722158862, 1882546615386653241, 2095045873215223828 },
  { 2457841790149984318, 7753875101144151848, -7295667086889139818, 7206668418931146560, -8078127443194693490, -5558314146826164806, -9030598288807633528 },
  { -3830120369918836531, -6016016492398343770, -2543820355150618957, -2158519753056713926, -1881172157569331057, -7666744224773783834, -4495834414709313344, 2018200285996995148, 5578912301471311906, 5644350214590911346, 3503331513761083257, 2618312331705133603, 4768226916359942699, -7469373568190958834, 1752215978860846971, 7945230559414143527, 1244523210340997451, 4401437662998893060, -4786365902114767257, -8679455487738158493, 3927615540674649527, 9176652918688865195, -5716603370273415004, 9125358142364169935, -4143977047845807666 },
  { -4191821234025004950, -6738010824921742175, -3989205420185496407, -3959402237761288907, -2614667578268996987, -8754656091029068319, -5220649015783760719, 1298181711861004600, 7439450795071509523, 4919546609486358591, 2421087544215739247, 2256611446039912473, 7876876231902375206 }
};

struct seen {
  int ncol,
      nrow;
  int cell_rows, cell_cols;
  int **data;
};

int delta[] = {3, 1<<16, 32, 0},pn,*pc;

struct seen mk_seen(int ncol, int nrow) {
  int nc = ncol + 2;
  int nr = nrow + 2;
  int **data = malloc(sizeof(int *) * nc);
  for (int i = 0; i < nc; i++) {
    data[i] = malloc(sizeof(int) * nr);
    memset(data[i], 1, sizeof(int) * nr);
    for (int j = 0; j < nr; j++)
      data[i][j] = rand() % delta[0] != 0;
  }
  struct seen s = {
    ncol, nrow, nr, nc, data
  };
  return s;
}

int PINTERP_LEN = 100;

int main(int argc, uint8_t **argv) {
  struct winsize __;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &__);
  int nc = __.ws_col -2;
  int nr = __.ws_row - 2;
  int **data = malloc(sizeof(int *) * nc);
  for (int i = 0; i < nc; i++) {
    data[i] = malloc(sizeof(int) * nr);
    memset(data[i], 1, sizeof(int) * nr);
    for (int j = 0; j < nr; j++)
      data[i][j] = rand() % delta[0] != 0;
  }
  struct seen s = {
    nc+2, nr+2, nr, nc, data
  };

  int TIMING=100000,*args[] = {&delta[0], &delta[1], &delta[2], &PINTERP_LEN, &TIMING, &delta[3]},idx,new[s.cell_cols][s.cell_rows],i,j,v,w,d,p,sum;
  long indices[] = {0x1b0048303b305b1b, 0x64253b353b38345b, 0x206d, 0x401040};
  for (int i = 1; i < argc; i++) {
    int val = atoi(argv[i]);
    if (val < 0) continue;
    *args[i - 1] = val;
  }

  pn = *(int *) &op[delta[3]][0];
  pc = 4 + (int *) &op[delta[3]][0];

  srand(time(0));
  char *x = "";
start:
  i=1;
i:
    j = 1;
    j:
    {
    sum = 0;
    for (v=0; v < 9;v++) sum += s.data[i + 1-v/3][j + 1-v%3] == 0;
    d = s.data[i][j];
    int k[] = {0, d > delta[2] ? 0 : rand() % delta[1] == 0 ? 0 : d + 1};
    p = 0;
    switch (sum) {
      case 2:if (d == 0) break;
      default:p=1;case 3:;
    }
    new[i][j] = k[p];
    j++;
    if (j < s.cell_rows - 1) goto j;
    }
    i++;
    if (i < s.cell_cols - 1) goto i;

  for (int i = 1; i < s.cell_cols - 1; i++)
    memcpy(&s.data[i][1], &new[i][1], 4 * (s.cell_rows - 2));

  int buffer[8 * s.cell_rows * s.cell_cols];
  uint8_t *pos = &buffer[0];
  fputs((uint8_t*)indices,stdout);
  for (int j = 0; j < s.cell_rows; j++) {
    for (int i = 0; i < s.cell_cols; i++) {
      idx = (int)(pn*(float) s.data[i][j] / (float) PINTERP_LEN);
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
