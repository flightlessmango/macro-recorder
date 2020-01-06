#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

struct timeval tp;
long int ms;
int last_change;
int change;

int getCurrentTimeMS(){
    gettimeofday(&tp, NULL);
    ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

void printEvent(XEvent event){
      printf("%s%x\n",(event.type == KeyPress?"+":"-"), event.xkey.keycode);
}

int main()
{
  Display *display;
  Window window;
  XEvent event;
  int s;

  /* open connection with the server */
  display = XOpenDisplay(NULL);
  if (display == NULL){
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  s = DefaultScreen(display);
  
  /* create window */
  window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 200, 200, 1,
                               BlackPixel(display, s), WhitePixel(display, s));

  /* select kind of events we are interested in */
  XSelectInput(display, window, KeyPressMask | KeyReleaseMask);
  /* map (show) the window */
  XMapWindow(display, window);

    /* map (show) the window */
 
    /* event loop */
    last_change = getCurrentTimeMS();
    while (1){
        XNextEvent(display, &event);
        
        /* keyboard events */
        if (event.type == KeyPress || event.type == KeyRelease){
          
          /* exit on ESC key press */
          if ( event.xkey.keycode == 0x09 )
              break;
          
          change = getCurrentTimeMS();
          printf("sleep %dms\n", change-last_change);
          last_change = change;
          printEvent(event);

        }
    }

    /* close connection to server */
    XCloseDisplay(display);
 
    return 0;
}