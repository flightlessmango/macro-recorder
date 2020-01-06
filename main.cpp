#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "X11/keysym.h"
#include <chrono>
// extern "C" {
//     #include <xdo.h>
// }

auto last_change = std::chrono::high_resolution_clock::now();
auto now = std::chrono::high_resolution_clock::now();
std::vector<XEvent> keyArray;
std::vector<XEvent> keyArray_prev;
// xdo_t * x = xdo_new(":0.0");
Display *display = XOpenDisplay(getenv("DISPLAY"));

void writeToFile(){
    printf("%i\n", keyArray_prev.size());
    FILE *outFile = fopen("/tmp/recording", "a");
    now = std::chrono::high_resolution_clock::now();  
    if (keyArray.empty()){
        fprintf(outFile, "%i\n", std::chrono::duration_cast<std::chrono::microseconds>(now - last_change));
        fflush(outFile);
    }

    fprintf(outFile, "%i\n", std::chrono::duration_cast<std::chrono::microseconds>(now - last_change));
    fflush(outFile);

    for (size_t i = 0; i < keyArray.size(); i++)
    {
        KeySym ks = XKeycodeToKeysym(display, keyArray[i].xkey.keycode, 1);
        if (i == keyArray.size() - 1){
            fprintf(outFile, "%s\n", XKeysymToString(ks));
        } else {
            fprintf(outFile, "%s,", XKeysymToString(ks));
        }
        fflush(outFile);
    }
    last_change = now;

}

int main()
{
    // while(1) {
        // xdo_send_keysequence_window_down(x, CURRENTWINDOW, "A", 0);
    // }

    Window window;
    XEvent event;
    Window root = DefaultRootWindow(display);
    Window curFocus;
    int revert;
    /* open connection with the server */
 
    XGetInputFocus (display, &curFocus, &revert);
    XSelectInput(display, curFocus, KeyPressMask | KeyReleaseMask | FocusChangeMask);
 
    
    /* event loop */
    while (1)
    {   
        keyArray_prev = keyArray;
        now = std::chrono::high_resolution_clock::now();
    // auto elapsed = std::chrono::high_resolution_clock::now() - start;

    // long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        XNextEvent(display, &event);
        /* keyboard events */
        if (event.type == KeyPress)
        {
            keyArray.push_back(event);
            printf( "KeyPress: %x\n", event.xkey.keycode );
            writeToFile();
        }
        else if (event.type == KeyRelease)
        {
            for (size_t i = 0; i < keyArray.size(); i++)
            {
                if (keyArray[i].xkey.keycode == event.xkey.keycode)
                    keyArray.erase(keyArray.begin() + i);
            }
            printf( "KeyRelease: %x\n", event.xkey.keycode );
            writeToFile();
        }
        else if (event.type == FocusOut)
        {
                // printf("%s\n", "Focus changed!");
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