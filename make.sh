#!/bin/sh

clang -DWNCK_I_KNOW_THIS_IS_UNSTABLE=1 $(pkg-config --cflags --libs gtk+-3.0) $(pkg-config --cflags --libs libwnck-3.0) -O2 window_switch.c -o window_switch
