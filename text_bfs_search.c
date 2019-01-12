#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char _int[] = {
               37, 99, 0,
               10, 0,
               71, 111, 111, 100, 98, 121, 101, 32, 58, 45, 41, 10, 0,
               27,  91, 72, 27, 91, 74, 0,
               27,  91, 37, 100, 59, 37, 100, 72, 0,
               27,  91, 37, 100, 109, 0, 0, 0,
               -30, -108, -125, 0,
               -30, -108, -127, 0,
               -30, -108, -109, 0,
               -30, -108, -113, 0,
               -30, -108, -101, 0,
               -30, -108, -105, 0,
               0x21,
               0x20
};
#define clear() 

__(a,m){
  m=a>>sizeof(int)*8-1;
  return (a^m)-m;
}
main() {
  void*y3=stdout,*s0=stdin,*(*mv)(size_t)=&malloc;
  int qq_,*hh,qf,(*g4)(void*)=&fflush,(*z1)(const char*,...)=&printf,(*p0)(int)=&putchar,
      c = 0,i,dd, h, s, e,pr,*trundle,*vp5,*nqd;
  char *p5;
goto cow;  
pic:g4(y3);
  int sz = dd*h;
  hh=mv(sizeof(int)*sz);
  z1(_int+18);
  for (int r = 0; r < sz; r++){z1(_int+25,1+ (r/dd), 1+r%dd);p0(p5[r]);}
  int oo[10];
  trundle=mv(sizeof(int)*sz);
  vp5=mv(sizeof(int)*sz);nqd=mv(sizeof(int)*sz);
  trundle[s] = 0;
  hh[0]=s;
  qf=1;qq_=0;
  st:;
  if(qq_==qf)goto ten;
  c=hh[qq_++];
    oo[0]=dd;oo[1]=h;oo[2]=c%dd;oo[3]=c/dd;oo[4]=0;
{      int x=oo[2],y=oo[3],dd=*oo,h=oo[1],i=x+y*dd;
      if (x+1<dd&&(p5[i+1]==32||p5[i+1]==33))oo[5+oo[4]++]=i+1;
      if (x-1>-1&&(p5[i-1]==_int[66]||p5[i-1]==_int[67]))oo[5+oo[4]++]=i-1;
      if (y+1<h&&p5[i+dd]==_int[67]||p5[i+dd]==_int[66])oo[5+oo[4]++]=i+dd;
      if (y-1>-1&&p5[i-dd]==_int[67]||p5[i-dd]==_int[66])oo[5+oo[4]++]=i-dd;
    }
    if (c&1?nqd[c]:vp5[c]) goto st;
    while (oo[4]-->0){
      i=oo[5+oo[4]];
      if ((i&1?vp5[i]:nqd[i]) == 0) {
        trundle[i]=c;
        usleep(100000);
        g4(y3);
	z1(_int+34, _int[67]);z1(_int+25,1+i/dd, 1+i%dd);p0(88);
        if (i==e)goto z;
        i&1?(vp5[i]=1):(nqd[i]=1);
        hh[qf++]=i;
      }
    }
    c&1?(nqd[c]=1):(vp5[c]=1);
    goto st;
  z:;
   z1(_int+34,36);z1(_int+25,1+e/dd,1+e%dd);p0(33);
   tww:;
   if (trundle[i]==s) goto ten;
      i=trundle[i];
      usleep(10000);
      z1(_int+34,35);z1(_int+25,1+i/dd,1+i%dd);p0(47);
    
    z1(_int+34,33);z1(_int+25,1+s/dd,1+s%dd);p0(95);
    goto tww;
ten:;
  z1(_int+25, h+3, 1);
  g4(y3);
  goto pio;
cow:;
{
  int c = 0;
  s=e=-1;
  char** strs=mv(1000),d;
  h=0;
 
  while(1) {
    char* s = mv(512);
    if (fgets(s, 512, s0) || feof(s0)) {
      dd=strlen(s)>dd?strlen(s):dd;
      strs[h] = s;
      h+=1;
      if(h==1000)break;
      if (feof(s0)&&(h--|1))break;
    } else goto ten;
  }
  p5 = mv(dd * h);
  for (int i = 0; i < dd*h; i += 1) p5[i]=040;
  for (int i = 0; i < h; i++) {
    memcpy(p5+i*dd, strs[i], strlen(strs[i]));
    for (int q = 0;((d=strs[i][q])||1)&&q<dd&&((d==64?s=i*dd+q:d==33?e=i*dd+q:d==10?p5[i*dd+q]=35:13)||1);q++);
  }
  if (s==-1||e==-1)goto ten;
goto pic;
}
pio:;
  }
