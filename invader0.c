/*
  From snotes-2018-b, 2019-01-30
  H(left), L(right), x (shot), z (finish)
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <string.h>
#include <stdio.h>

static const char *event_names[] = {
                                    "",
                                    "",
                                    "KeyPress",
                                    "KeyRelease",
                                    "ButtonPress",
                                    "ButtonRelease",
                                    "MotionNotify",
                                    "EnterNotify",
                                    "LeaveNotify",
                                    "FocusIn",
                                    "FocusOut",
                                    "KeymapNotify",
                                    "Expose",
                                    "GraphicsExpose",
                                    "NoExpose",
                                    "VisibilityNotify",
                                    "CreateNotify",
                                    "DestroyNotify",
                                    "UnmapNotify",
                                    "MapNotify",
                                    "MapRequest",
                                    "ReparentNotify",
                                    "ConfigureNotify",
                                    "ConfigureRequest",
                                    "GravityNotify",
                                    "ResizeRequest",
                                    "CirculateNotify",
                                    "CirculateRequest",
                                    "PropertyNotify",
                                    "SelectionClear",
                                    "SelectionRequest",
                                    "SelectionNotify",
                                    "ColormapNotify",
                                    "ClientMessage",
                                    "MappingNotify"
};

struct my_ch {
  int xx;
  int yy;
  int sflag[3];
  int sx[3];
  int sy[3];
  int vec;
};

int main(int argc, char** argv) {
  Display* display = XOpenDisplay(NULL);
  if (display == NULL) {
    return 1;
  }

  int screen = DefaultScreen(display);

  GC gc = DefaultGC(display, screen);

  Window parent_window = DefaultRootWindow(display);

  int x = 0;
  int y = 0;

  unsigned int width = 600;
  unsigned int height = 600;

  unsigned int border_width = 1;

  unsigned int border_color = BlackPixel(display, screen);
  unsigned int background_color = WhitePixel(display, screen);

  // Create window
  Window hello_window = XCreateSimpleWindow(display, parent_window,
                                            x,
                                            y,
                                            width,
                                            height,
                                            border_width,
                                            border_color,
                                            background_color);

  long event_mask = ExposureMask
    | KeyPressMask
    | KeyReleaseMask
    | ButtonPressMask
    | ButtonReleaseMask
    | FocusChangeMask
    ;

  // Select window events
  XSelectInput(display, hello_window, event_mask);

  // Make window visible
  XMapWindow(display, hello_window);

  // Set window title
  XStoreName(display, hello_window, "test window");

  // Get WM_DELETE_WINDOW atom
  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", True);

  // Subscribe WM_DELETE_WINDOW message
  XSetWMProtocols(display, hello_window, &wm_delete, 1);

  char msg[1024] = "";
  char key[32];


  int i;
  int j;
  int cnt = 0;
  int end = 0;
  struct my_ch my;
  struct my_ch enemy;
  XEvent event;

  // initialize
  my.xx = 300;
  my.yy = 500;
  enemy.xx = 100;
  enemy.yy = 100;
  enemy.vec = 0;
  for(i = 0; i < 3; i++) {
    my.sflag[i] = 0;
    my.sx[i] = my.xx;
    my.sy[i] = my.yy - 5;
  }
    
  // Event loop
  for (;;) {

    if(XPending(display)) {
      // Get next event from queue
      XNextEvent(display, &event);
      // Print event type
      printf("got event: %s\n", event_names[event.type]);
      

        
        
      // Keyboard
      if (event.type == KeyPress) {
        int len = XLookupString(&event.xkey, key, sizeof(key) - 1, 0, 0);
        key[len] = 0;

        if (strlen(msg) > 50)
          msg[0] = 0;

        for(i = 0; i < len; i++) {
          if(key[i] == 'h') {
            my.xx -= 4;
            //strcat(msg, key);
            break;
          }
          if(key[i] == 'l') {
            my.xx += 4;
            //strcat(msg, key);
            break;
          }
          if(key[i] == 'x') {
            strcat(msg, key);
            for(j = 0; j < 3; j++) {
              if(my.sflag[j] == 0) {
                my.sx[j] = my.xx;
                my.sy[j] = my.yy - 5;
                my.sflag[j] = 1;
                break;
              }
            }
            break;
          }

          // zボタンで終了
          if(key[i] == 'z') {
            //strcat(msg, key);
            end = 1;
            break;
          }
        }

        //strcat(msg, key);
        //strcat(msg, " ");
      }
      
      
      // 終了処理
      if(end == 1) {
        break;
      }

      // Refresh
      if (event.type == KeyPress || event.type == Expose) {
        XClearWindow(display, hello_window);
        //XDrawString(display, hello_window, gc, 10, 20, msg, strlen(msg));

        // draw my-charcter
        XDrawString(display, hello_window, gc, my.xx, my.yy, "t", 1);

        // draw shot
        for(j = 0; j < 3; j++) {
          if(my.sflag[j] == 1) {
            XDrawString(display, hello_window, gc, my.sx[j], my.sy[j], "i", 1);
            my.sy[j] -= 1;
            if(my.sy[j] == 0) {
              my.sflag[j] = 0;
            }
          }
        }
      }
    }

    // draw shot
    if(cnt%4000 == 0) {
      XClearWindow(display, hello_window);
      //XDrawString(display, hello_window, gc, 10, 20, msg, strlen(msg));
      // draw my-charcter
      XDrawString(display, hello_window, gc, my.xx, my.yy, "t", 1);
      for(j = 0; j < 3; j++) {
        if(my.sflag[j] == 1) {
          my.sy[j] -= 1;
          XDrawString(display, hello_window, gc, my.sx[j], my.sy[j], "i", 1);
          if(my.sy[j] <= 0) {
            my.sflag[j] = 0;
          }
        }
      }
    }

    // draw enemy
    XDrawString(display, hello_window, gc, enemy.xx, enemy.yy, "Y", 1);
    if(cnt%10000 == 0) {
      XClearWindow(display, hello_window);
      //XDrawString(display, hello_window, gc, 10, 20, msg, strlen(msg));
      // draw my-charcter
      XDrawString(display, hello_window, gc, my.xx, my.yy, "t", 1);
      if(enemy.vec == 0) {
        enemy.xx ++;
        XDrawString(display, hello_window, gc, enemy.xx, enemy.yy, "Y", 1);
        if(enemy.xx >= 595) {
          enemy.vec = 1;
        }
      }
      else {
        enemy.xx --;
        XDrawString(display, hello_window, gc, enemy.xx, enemy.yy, "Y", 1);
        if(enemy.xx <= 5) {
          enemy.vec = 0;
        }
      }
    }
      
    cnt ++;
        
    // 当たり判定と終了処理
    for(j = 0; j < 3; j++) {
      if(enemy.xx >= my.sx[j]-2 && enemy.xx <= my.sx[j]+5) {
        if(enemy.yy >= my.sy[j]-5 && enemy.yy <= my.sy[j]+2) {
          end = 1;
        }
      }
    }
    // 終了処理
    if(end == 1) {
      break;
    }
      
    // Close button
    if (event.type == ClientMessage) {
      if (event.xclient.data.l[0] == wm_delete) {
        break;
      }
    }
  }

  XCloseDisplay(display);
  return 0;
}
