#include <stdio.h>
#include <unistd.h>
typedef struct n{int i; int p;}n;
typedef struct q{
  n* h;
  int l;
}q;
q* mkq(int s) {
  struct q*r=malloc(sizeof(struct q));
  r->h=malloc(s<<3);
  r->l=0;
  return r;
}
void nq(q*qq, int i, int p) {
  qq->h[qq->l].i=i;qq->h[i=qq->l++].p=p;
  if(i){
  int j;
  while (qq->h[i].p<qq->h[j=(i-1)/2].p){
    //qq->h[j]^=(qq->h[i^=j^=i^=j]^=(qq->h[i^=j^=i^=j]^=qq->h[i^=j^=i^=j]));
    qq->h[j].p^=(qq->h[i].p^=(qq->h[j].p^=qq->h[i].p));
    qq->h[j].i^=(qq->h[i].i^=(qq->h[j].i^=qq->h[i].i));
    i=j;
  }}
}
int poll(q*qq) {
  int r = qq->h[0].i,a,c,d,z;
  switch(qq->l--){
  default:
    a=0;
    qq->h[0].p=qq->h[qq->l].p;qq->h[0].i=qq->h[qq->l].i;
    LLL:;
    if (a>=qq->l) goto L;
      d=1+(c=(a<<1)+1);
      if (c<qq->l) {
        if(d<qq->l) {
          if (qq->h[d].p>qq->h[c].p) {z=c;}
          else {z=d;}
          if (qq->h[z].p<qq->h[a].p) {
            goto LL;
          } else { goto L; }
        } else{
          if(qq->h[z=c].p<qq->h[a].p){
            goto LL;
          } else { goto L; }
        }
      } else {a=c;}
      goto LLL;
    LL:;
      qq->h[z].p^=qq->h[a].p^=qq->h[z].p^=qq->h[a].p;
      qq->h[z].i^=qq->h[a].i^=qq->h[z].i^=qq->h[a].i;
      a=z;
      goto LLL;
  case 1:
  L:;
    return r;
  case 0:
    return -1;
  }
}



void readmap(int* width, int* height, int* start, int*end, char** omap) {
  // figure out w and h from the input
  int c = 0;
  *start=*end=-1;
  char** strs=malloc(8*1000);
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
  char *buf = map;
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
#define setclr(clr) printf(_int+34, clr)
#define movecursor(x, y) printf(_int+25, y, x)
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
  int c = 0,i;
  int w, h, s, e,pr;
  char *map;
  readmap(&w, &h, &s, &e, &map);
  fflush(stdout);
  int sz = w*h;
  printf(_int+18);
  for (int r = 0; r < sz; r++){movecursor(1+ (r%w), 1+r/w);printf("%c", map[r]);}
  int nbrs[10];
  q* qq = mkq(sz);
  int* trail=malloc(sizeof(int)*sz);
  int* vmap=malloc(sizeof(int)*sz),*nqd=malloc(sizeof(int)*sz),*dmap=malloc(sizeof(int)*sz);
  trail[s] = 0;
  nq(qq, s, 0);

  while ((c=poll(qq))!=-1) {
    nbrs[0]=w;nbrs[1]=h;nbrs[2]=c%w;nbrs[3]=c/w;nbrs[4]=0;
    nbr(map, nbrs);
    if (c&1?nqd[c]:vmap[c]) continue;
    while (nbrs[4]-->0){
      i=nbrs[5+nbrs[4]];
      if ((i&1?vmap[i]:nqd[i]) == 0) {
        trail[i]=c;
        usleep(10000);
        printf(_int+34, _int[67]);printf(_int+25,1+i/w, 1+i%w);printf("+", map[i]);
        if (i==e)goto z;
        i&1?(vmap[i]=1):(nqd[i]=1);
        dmap[i]=dmap[c]+1;
        pr= (__(i/w-e/w,4) + __(i%w-e%w,6))*2/3 + dmap[i];
        dmap[i]=pr;
        nq(qq, i,  pr);
      }
    }
    c&1?(nqd[c]=1):(vmap[c]=1);
    continue;
  z:;
    setclr(36);printf(_int+25,1+e/w,1+e%w);printf("!",map[s]);
    while (trail[i]!=s) {
      i=trail[i];
      usleep(10000);
      setclr(35);printf(_int+25,1+i/w,1+i%w);printf("/",map[i]);
    }
    setclr(33);printf(_int+25,1+s/w,1+s%w);printf("_",map[s]);
    break;
  }
  movecursor(1, h+3);
}
