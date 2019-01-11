#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

char _int[] = {
  // "%c" = 0
  37, 99, 0,
  // "\n" = 3
  10, 0,
  // "G   o    o    d    b   y    e    _   :   -   )   \n" = 5
      71, 111, 111, 100, 98, 121, 101, 32, 58, 45, 41, 10, 0,
  // "\033 [   H   \033 [   J" = 18
      27,  91, 72, 27, 91, 74, 0,
  // "\033 [   %   d    ;   %   d    H" = 25
      27,  91, 37, 100, 59, 37, 100, 72, 0,
  // "\033 [   %   d    m    %   s" = 34
      27,  91, 37, 100, 109, 37, 115, 0,
  // "┃" = 42
  -30, -108, -125, 0,
  // "━" = 46
  -30, -108, -127, 0,
  // "┓" = 50
  -30, -108, -109, 0,
  // "┏" = 54
  -30, -108, -113, 0,
  // "┛" = 58
  -30, -108, -101, 0,
  // "┗" = 62
  -30, -108, -105, 0
};
#define clearscreen() printf(&_int[18])
#define movecursor(x, y) printf(&_int[25], y, x)
#define printcoloredstr(ch, color) printf(&_int[34], color, ch)
#define randbool() (rand()&1)
int ROWS = 24;
int COLS = 80;

#define BACKGROUNDCOLORS 0

#define WHITE 37+BACKGROUNDCOLORS
#define CYAN 36+BACKGROUNDCOLORS
#define MAGENTA 35+BACKGROUNDCOLORS
#define BLUE 34+BACKGROUNDCOLORS
#define YELLOW 33+BACKGROUNDCOLORS
#define GREEN 32+BACKGROUNDCOLORS
#define RED 31+BACKGROUNDCOLORS

#define U 0
#define R 1
#define D 2
#define L 3


/*
  0 -> UP/DOWN => UP/DOWN
  1 -> LEFT/RIGHT => LEFT/RIGHT
  2 -> UP/RIGHT => LEFT/DOWN
  3 -> UP/LEFT => RIGHT/DOWN
  4 -> DOWN/LEFT => LEFT/UP
  5 -> DOWN/RIGHT => RIGHT/UP
 */
//char* charSets[] = { { 191, 0 }, "-", "\\", "/", "/", "\\"};
char* charSets[] = {&_int[42], &_int[46], &_int[50], &_int[54], &_int[58], &_int[62]};
int charset = 0;
int nPipes = 2;
int colorMode = 0;

int getColor() {
  return 31 + (rand() % 7);
}

#define X_IND 0
#define Y_IND 1
#define COLOR_IND 2
#define TRACK_IND 3
#define DIR_IND 4
#define LDIR_IND 5
#define TURNS_IND 6
#define ROWS_IND 7
#define COLS_IND 8

typedef struct Pipe {
  int x, y, color, track;
  int direction, lastDirection;
} Pipe;

int resetPipe(int p[]) {
  if (randbool()) {
    if (randbool()) {
      p[Y_IND] = 0;
      p[DIR_IND] = D;
    } else {
      p[Y_IND] = p[ROWS_IND];
      p[DIR_IND] = U;
    }
    p[X_IND] = rand() % (p[COLS_IND]-1);
  } else {
    if (randbool()) {
      p[X_IND] = 0;
      p[DIR_IND] = R;
    } else {
      p[X_IND] = p[COLS_IND];
      p[DIR_IND] = L;
    }
    p[Y_IND] = rand() % (p[ROWS_IND]-1);
  }
  p[LDIR_IND] = p[DIR_IND];
  p[TRACK_IND] = 2 + (rand() % 5);
  p[COLOR_IND] = getColor();
  return 1;
}

void turnPipe(int p[]) {
  p[TURNS_IND]--;
  p[LDIR_IND] = p[DIR_IND];
  if ((rand() & 3) == 0) {
    switch(p[DIR_IND]) {
    case U:
    case D:
      if (randbool()) p[DIR_IND] = L;
      else if (randbool()) p[DIR_IND] = R;
      break;
    case L:
    case R:
      if (randbool()) p[DIR_IND] = U;
      else if (randbool()) p[DIR_IND] = D;
    }
  }
  p[TRACK_IND] = 2 + (rand() % 8);
}

/*
  0 -> UP/DOWN => UP/DOWN
  1 -> LEFT/RIGHT => LEFT/RIGHT
  2 -> UP/RIGHT => LEFT/DOWN
  3 -> UP/LEFT => RIGHT/DOWN
  4 -> DOWN/LEFT => LEFT/UP
  5 -> DOWN/RIGHT => RIGHT/UP
*/
void printPipe(int p[]) {
  // U R D L
  // 0 1 2 3
  static const int map[][4] = {  {0, 3, 0, 2},
                                 {4, 1, 2, 1},
                                 {0, 5, 0, 4},
                                 {5, 1, 3, 1} };
  char* c = charSets[map[p[LDIR_IND]][p[DIR_IND]]];
  movecursor(p[X_IND], p[Y_IND]);
  printcoloredstr(c, p[COLOR_IND]);
  p[LDIR_IND] = p[DIR_IND];
}

int updatePipe(int p[], int* ts) {
  *ts-=1;
  if (*ts <= 0) {
    *ts = 0;
  }
  p[TRACK_IND]--;
  if (p[TRACK_IND] <= 0) {
    turnPipe(p);
  } else {
    switch(p[DIR_IND]) {
    case U:
      p[Y_IND] -= 1;
      break;
    case D:
      p[Y_IND] += 1;
      break;
    case L:
      p[X_IND] -= 1;
      break;
    case R:
      p[X_IND] += 1;
    }
    if (p[X_IND] <= 0 | p[X_IND] >= p[COLS_IND] |
        p[Y_IND] <= 0 | p[Y_IND] >= p[ROWS_IND]) {
      resetPipe(p);
    }
  }
  printPipe(p);
  return 1;
}

int main(int argc, char** args) {


  int pipe[28];
  pipe[27]=0;
  while (pipe[27] < 21 && resetPipe(&pipe[pipe[27]]) && (pipe[27]+=9));
  pipe[ROWS_IND] = argc > 1 ? atoi(args[1]) : 24;
  pipe[COLS_IND] = argc > 2 ? atoi(args[1]) : 80;
  pipe[9+ROWS_IND]=pipe[18+ROWS_IND]=pipe[ROWS_IND];
  pipe[9+COLS_IND]=pipe[18+COLS_IND]=pipe[COLS_IND];
  pid_t pid = fork();

  if (pid) {
    char _unused;
    scanf(_int, &_unused);
    kill(pid, SIGKILL);
    clearscreen();
    printcoloredstr(&_int[3], 39);
    printf(&_int[5]);
  } else {
    srand(time(NULL));
    struct timeval _1, _2;
    struct timespec t1 = {.tv_sec = 0, .tv_nsec = 1000000000 / 35 }, t2;
    int tn = 1000000000 / 35, p = (pipe[ROWS_IND]+pipe[COLS_IND])*40;
    clearscreen();
    while (1) {
      gettimeofday(&_1, 0);
      pipe[27]^=pipe[27];
      while (pipe[27] < 21 && updatePipe(&pipe[pipe[27]++], &p) && (pipe[27]+=8));
      if (p==0) {
        pipe[27]^=pipe[27];
        while (pipe[27] < 21 && resetPipe(&pipe[pipe[27]]) && (pipe[27]+=9));
        clearscreen();
        p=(pipe[ROWS_IND]+pipe[COLS_IND])*40;
      }
      movecursor(0, 0);
      fflush(stdout);
      gettimeofday(&_2, 0);
      t1.tv_nsec = tn - (_2.tv_usec-_1.tv_usec) * 1000;
      fflush(stdout);
      nanosleep(&t1, &t2);
    }
  }
}
