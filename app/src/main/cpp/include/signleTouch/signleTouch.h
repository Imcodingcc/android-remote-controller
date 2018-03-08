#ifndef SIGNLETOUCH_H
#define SIGNLETOUCH_H
extern "C" {
void sendScreenTouch(int fd, int startX, int startY, int endX, int endY);
void nvr_execute_down(int fd, int startX, int startY);
void nvr_execute_move(int fd, int endX, int endY);
void nvr_execute_up(int fd);
};
#endif
