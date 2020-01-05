#include <X11/Xlib.h>
#include <iostream>
#include "X11/keysym.h"

Display *dpy = XOpenDisplay(":0");

bool key_is_pressed(KeySym ks) {
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}

bool ctrl_is_pressed() {
    return key_is_pressed(XK_Control_L) || key_is_pressed(XK_F12);
}

int main(int argc, char **argv) {
  while(1){
        if (ctrl_is_pressed() == 1) {
          std::cout << ctrl_is_pressed() << std::endl;
        }
  }

    return (0);
};