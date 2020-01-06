#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

struct timeval tp;
long int ms;
int last_change;

int getTime(){
    gettimeofday(&tp, NULL);
    ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

int main()
{
    Display *display = XOpenDisplay(getenv("DISPLAY"));
    Window window;
    XEvent event;
    Window root = DefaultRootWindow(display);
    Window curFocus;
    int revert;
    /* open connection with the server */
 
    XGetInputFocus (display, &curFocus, &revert);
    XSelectInput(display, curFocus, KeyPressMask | KeyReleaseMask | FocusChangeMask);
 
    /* map (show) the window */
 
    /* event loop */
    while (1)
    {
        XNextEvent(display, &event);
        /* keyboard events */
        if (event.type == KeyPress)
        {
            last_change = getTime();
            printf( "KeyPress: %x\n", event.xkey.keycode );

            /* exit on ESC key press */
            if ( event.xkey.keycode == 0x09 )
                break;
        }
        else if (event.type == KeyRelease)
        {
            last_change = getTime();
            printf( "KeyRelease: %x\n", event.xkey.keycode );
        }
        else if (event.type == FocusOut)
        {
                printf("%s\n", "Focus changed!");
                if (curFocus != root)
                    XSelectInput(display, curFocus, 0);
                XGetInputFocus (display, &curFocus, &revert);

                if (curFocus == PointerRoot)
                    curFocus = root;
                XSelectInput(display, curFocus, KeyPressMask|KeyReleaseMask|FocusChangeMask);
        }
    }

    /* close connection to server */
    XCloseDisplay(display);
 
    return 0;
}