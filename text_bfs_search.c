#include <stdio.h>
#include <unistd.h>
void readmap(int* width, int* height, int* start, int*end, char** omap) {
  // figure out w and h from the input
  int c = 0;
  *start=*end=-1;
  char** strs=malloc(1000);
  *height=0;
  while(1) {
    char* s = malloc(512);
    if (fgets(s, 512, stdin) || feof(stdin)) {
      *width=strlen(s)>*width?strlen(s):*width;
      strs[*height] = s;
      *height+=1;
      if (feof(stdin)) {*height-=1;break;}
    } else exit(0);
  }
  char* map = malloc(*width * *height);
  for (int i = 0; i < *width**height; i += 1) map[i]=' ';
  for (int i = 0; i < *height; i++) {
    memcpy(map+i**width, strs[i], strlen(strs[i]));
    for (int w = 0; w<*width;w++)
      switch (strs[i][w]) {
      case '@': *start=i**width+w;break;
      case '!': *end=i**width+w;
      }
  }
  if (*start==-1||*end==-1) { exit(0); }
  *omap = map;
}
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
               27,  91, 37, 100, 109, 0, 0, 0,
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
               -30, -108, -105, 0,
               // '!' = 66
               0x21,
               // ' ' =67
               0x20
};
#define clear() 
// o: [w, h, x, y, n, 1, 2, 3, 4]
int nbr(char*map, int* out) {
  int x=out[2],y=out[3],w=*out,h=out[1],i=x+y*w;
  if (x+1<w&&(map[i+1]==32||map[i+1]==33))out[5+out[4]++]=i+1;
  if (x-1>-1&&(map[i-1]==_int[66]||map[i-1]==_int[67]))out[5+out[4]++]=i-1;
  if (y+1<h&&map[i+w]==_int[67]||map[i+w]==_int[66])out[5+out[4]++]=i+w;
  if (y-1>-1&&map[i-w]==_int[67]||map[i-w]==_int[66])out[5+out[4]++]=i-w;
}
__(a,m){
  m=a>>sizeof(int)*8-1;
  return (a^m)-m;
}
int main() {
  void*sout=stdout,*sin=stdin;
  int qql,*qqh,qqf,(*flus)(void*),(*z1)(const char*,...)=&printf,(*pch)(int)=&putchar;
  int c = 0,i;
  int w, h, s, e,pr;
  char *map;
  readmap(&w, &h, &s, &e, &map);
  fflush(stdout);
  int sz = w*h;
  qqh=malloc(sizeof(int)*sz);
  z1(_int+18);
  for (int r = 0; r < sz; r++){z1(_int+25,1+ (r/w), 1+r%w);pch(map[r]);}
  int nbrs[10];
  int* trail=malloc(sizeof(int)*sz);
  int* vmap=malloc(sizeof(int)*sz),*nqd=malloc(sizeof(int)*sz);
  trail[s] = 0;
  qqh[0]=s;
  qqf=1;qql=0;
  st:;
  if(qql==qqf)goto end;
  c=qqh[qql++];
  //while ((c=poll(qq))!=-1) {
    nbrs[0]=w;nbrs[1]=h;nbrs[2]=c%w;nbrs[3]=c/w;nbrs[4]=0;
    nbr(map, nbrs);
    if (c&1?nqd[c]:vmap[c]) goto st;
    while (nbrs[4]-->0){
      i=nbrs[5+nbrs[4]];
      if ((i&1?vmap[i]:nqd[i]) == 0) {
        trail[i]=c;
        usleep(100000);
        fflush(stdout);
	z1(_int+34, _int[67]);z1(_int+25,1+i/w, 1+i%w);putchar(88);
        if (i==e)goto z;
        i&1?(vmap[i]=1):(nqd[i]=1);
        qqh[qqf++]=i;
      }
    }
    c&1?(nqd[c]=1):(vmap[c]=1);
    goto st;
  z:;
   z1(_int+34,36);z1(_int+25,1+e/w,1+e%w);putchar(33);
    while (trail[i]!=s) {
      i=trail[i];
      usleep(10000);
      z1(_int+34,35);z1(_int+25,1+i/w,1+i%w);putchar(47);
    }
    z1(_int+34,33);z1(_int+25,1+s/w,1+s%w);putchar(95);
    goto end;
end:;
  z1(_int+25, h+3, 1);
  fflush(stdout);
}
