/* 16x16 box を描きいろんな場所に貼り付ける 
   misc-2022/11/4nen-m1/xpic4-move.c
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#define MILLI_SECCONDS 1000000
void mysleep() {
  /* sleep(1); nanosleep が使えない場合 */
  struct timespec stopTime={0, 200*MILLI_SECCONDS};
  nanosleep(&stopTime,NULL);
}

int main() {
  Display *d;
  Window r,w;
  Pixmap pix;
  Pixmap pixBlack;
  GC gc;
  XSetWindowAttributes a;
  int s;
  int i,size,x,y;

  d = XOpenDisplay(NULL);

  r = RootWindow(d,0);
  w = XCreateSimpleWindow(d,r,50,50,400,300,2,0,1);
  a.override_redirect = 1;
  XChangeWindowAttributes(d,w,CWOverrideRedirect, &a);
  XMapWindow(d,w);

  s = DefaultScreen(d);
  gc = DefaultGC(d,s);

  pix = XCreatePixmap(d,w,400,300,24); /* 仮想 screen, w には意味なし. 24bit color*/
  pixBlack = XCreatePixmap(d,w,400,300,24); /* 仮想 screen, 消すためのデータ */

  /* 仮想スクリーンに box を描く */
  size=16;
  for (i=1; i<=size; i++) {
    XSetForeground(d,gc,0xff0000); /* 色指定. 赤で描く */
    XDrawPoint(d,pix,gc,1,i);
    XDrawPoint(d,pix,gc,size,i);
    XDrawPoint(d,pix,gc,i,1);
    XDrawPoint(d,pix,gc,i,size);
  }      

  /* pix から表示されてる window w へコピー.
    http://cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/XLIB/xlib6.html
  */
  XSetGraphicsExposures(d,gc,False);

  while (1) {
    x = random() % 400;
    y = random() % 300;
    XCopyArea(d,pix,w,gc,1,1,size,size, x,y);  /* 描く */
    XFlush(d);
    mysleep();
    XCopyArea(d,pixBlack,w,gc,1,1,size,size, x,y); /* 消す */
  }
    
  XFlush(d);
  XFreePixmap(d,pix);
  XFreePixmap(d,pixBlack);
}
