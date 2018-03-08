#ifndef UINPUT_H
#define UINPUT_H

#define KEYDOWN 1
#define KEYUP 0
#define TOUCHDOWN 1
#define TOUCHUP 0
#define ROTATING_90 1
#define ROTATING_180 2
#define ROTATING_270 3

int createMouseDevice();
int createTouchScreen();
int createTouchDevice(int sw, int sy);
int handleMouseMove(int RelX, int RelY);
int handleMouseKey(int status, int keycode);
int handleTouch(int type,int x,int y,int ori,int touchcode);
void test();
#endif