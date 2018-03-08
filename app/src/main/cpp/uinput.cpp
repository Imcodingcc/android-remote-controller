#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <linux/input.h>
#include <linux/uinput.h>
#include <time.h>
#include <iostream>
#include <uinput.h>
#include <cutils/properties.h>

using namespace std;

#define die(str, args...)   \
    do                      \
    {                       \
        perror(str);        \
        exit(EXIT_FAILURE); \
    \
} while (0)

int fd_mouse;
int fd_touch;
int width;
int height;
int uinp_fd;

int createMouseDevice(){
    struct uinput_user_dev uinp;
    fd_mouse = open("/dev/uinput", O_WRONLY | O_NDELAY);
    if(fd_mouse <= 0)
    {
        return -1;
    }
    memset(&uinp, 0x00, sizeof(uinp));
    strncpy(uinp.name, "myMouse", strlen("myMouse"));
    uinp.id.version = 1;
    uinp.id.bustype = BUS_USB;

    ioctl(fd_mouse, UI_SET_EVBIT, EV_KEY);
    ioctl(fd_mouse, UI_SET_EVBIT, EV_REL);
    ioctl(fd_mouse, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(fd_mouse, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd_mouse, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd_mouse, UI_SET_RELBIT, REL_X);
    ioctl(fd_mouse, UI_SET_RELBIT, REL_Y);

    if(write(fd_mouse, &uinp, sizeof(uinp)) != sizeof(uinp))
    {
        close(fd_mouse);
        fd_mouse = -1;
        return fd_mouse;
    }

    if(ioctl(fd_mouse, UI_DEV_CREATE))
    {
        close(fd_mouse);
        fd_mouse = -1;
        return fd_mouse;
    }
    return fd_mouse;

}

int handleMouseMove(int RelX, int RelY){
    if(fd_mouse<0){
        return -1;
    }
    int ret=-1;
    static struct input_event ievent[3];
    static struct timespec now;
    float tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &now);
    tv_nsec = now.tv_nsec / 1000;

    ievent[0].time.tv_sec = now.tv_sec;
    ievent[0].time.tv_usec = tv_nsec;
    ievent[0].type = EV_REL;
    ievent[0].code = REL_X;
    ievent[0].value = RelX;

    ievent[1].time.tv_sec = now.tv_sec;
    ievent[1].time.tv_usec = tv_nsec;
    ievent[1].type = EV_REL;
    ievent[1].code = REL_Y;
    ievent[1].value = RelY;

    ievent[2].time.tv_sec = now.tv_sec;
    ievent[2].time.tv_usec = tv_nsec;
    ievent[2].type = EV_SYN;
    ievent[2].code = 0;
    ievent[2].value = 0;

    ret=write(fd_mouse, &ievent[0], sizeof(ievent[0]));
    ret=write(fd_mouse, &ievent[1], sizeof(ievent[1]));
    ret=write(fd_mouse, &ievent[2], sizeof(ievent[2]));

    return ret;
}

int handleMouseKey(int status, int keycode){
    if(fd_mouse<0){
        return -1;
    }
    int ret=-1;
    struct input_event event;
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);

    event.type = EV_KEY;
    event.code = keycode;
    event.value = status;
    write(fd_mouse, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = 0;
    event.value = 0;
    ret=write(fd_mouse, &event, sizeof(event));

    return ret;
}

int createTouchDevice(int sw,int sh){
    struct uinput_user_dev uinp;
    width= sw;
    height= sh;

    fd_touch= open("/dev/uinput", O_WRONLY | O_NDELAY);
    if(fd_touch <= 0)
    {
        return false;
    }
    memset(&uinp, 0x00, sizeof(uinp));
    strncpy(uinp.name, "myTouch", strlen("myTouch"));
    uinp.id.version = 1;
    uinp.id.bustype = BUS_USB;

    uinp.absmin[ABS_MT_POSITION_X] = 0;
    uinp.absmax[ABS_MT_POSITION_X] = sw;
    uinp.absfuzz[ABS_MT_POSITION_X] = 0;
    uinp.absflat[ABS_MT_POSITION_X] = 0;

    uinp.absmin[ABS_MT_POSITION_Y] = 0;
    uinp.absmax[ABS_MT_POSITION_Y] = sh;
    uinp.absfuzz[ABS_MT_POSITION_Y] = 0;
    uinp.absflat[ABS_MT_POSITION_Y] = 0;

    uinp.absmin[ABS_MT_PRESSURE] = 0;
    uinp.absmax[ABS_MT_PRESSURE] = 1;
    uinp.absfuzz[ABS_MT_PRESSURE] = 0;
    uinp.absflat[ABS_MT_PRESSURE] = 0;
    uinp.absmin[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmax[ABS_MT_TOUCH_MAJOR] = 31;
    uinp.absfuzz[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absflat[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmin[ABS_MT_SLOT] = 0;
    //uinp.absmax[ABS_MT_SLOT] = MT_SLOT_T;
    uinp.absmax[ABS_MT_SLOT] = 0;
    uinp.absfuzz[ABS_MT_SLOT] = 0;
    uinp.absflat[ABS_MT_SLOT] = 0;
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    //uinp.absmax[ABS_MT_TRACKING_ID] = MT_SLOT_T;
    uinp.absmax[ABS_MT_TRACKING_ID] = 0;
    uinp.absfuzz[ABS_MT_TRACKING_ID] = 0;
    uinp.absflat[ABS_MT_TRACKING_ID] = 0;

    ioctl(fd_touch, UI_SET_EVBIT, EV_ABS);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_SLOT);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_PRESSURE);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(fd_touch, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(fd_touch, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

    if(write(fd_touch, &uinp, sizeof(uinp)) != sizeof(uinp))
    {
        close(fd_touch);
        fd_touch = -1;
        return fd_touch;
    }

    if(ioctl(fd_touch, UI_DEV_CREATE))
    {
        close(fd_touch);
        fd_touch = -1;
        return fd_touch;
    }
    return 1;
}

void adapterxy(int &x,int &y,int ori)
{
    int tx = x,ty = y;
    switch(ori)
    {
        case ROTATING_90:
            x  =  width- ty;
            y  =  tx;
            break;
        case ROTATING_180:
            x = width - tx;
            y = height- ty;
            break;
        case ROTATING_270:
            x = ty;
            y = height -x;
            break;
    }
}

int handleTouch(int type,int x,int y,int ori,int touchcode){
    struct input_event event;

    if(fd_touch<0){
        if(!createTouchDevice(width,height))
        {
            return -1;
        }
    }

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);

    switch (type){
        case TOUCHDOWN:               //Touch
        {
            adapterxy(x,y,ori);//test
            event.type = EV_ABS;
            event.code = ABS_MT_SLOT;
            event.value = touchcode;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_ABS;
            event.code = ABS_MT_TRACKING_ID;
            event.value = touchcode;
            write(fd_touch, &event, sizeof(event));


            event.type = EV_ABS;
            event.code = ABS_MT_TOUCH_MAJOR;
            event.value = 5;
            write(fd_touch, &event, sizeof(event));


            event.type = EV_ABS;
            event.code = ABS_MT_PRESSURE;
            event.value = 1;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_ABS;
            event.code = ABS_MT_POSITION_X;
            event.value = x;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_ABS;
            event.code = ABS_MT_POSITION_Y;
            event.value = y;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_SYN;
            event.code = SYN_REPORT;
            event.value = 0;
            write(fd_touch, &event, sizeof(event));
        }
            break;

        case TOUCHUP:                //Left
        {
            event.type = EV_ABS;
            event.code = ABS_MT_SLOT;
            event.value = touchcode;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_ABS;
            event.code = ABS_MT_TRACKING_ID;
            event.value = -1;
            write(fd_touch, &event, sizeof(event));

            event.type = EV_SYN;
            event.code = SYN_REPORT;
            event.value = 0;
            write(fd_touch, &event, sizeof(event));
        }
            break;

        default:
            break;
    }
    return 1;
}
int createTouchScreen()
{
    struct uinput_user_dev uinp;
    struct input_event event;
    uinp_fd = open("/dev/uinput", O_WRONLY|O_NONBLOCK);
    if(uinp_fd == 0) {
        //ALOGD("Unable to open /dev/uinput\n");
        return -1;
    }

    // configure touch device event properties
    memset(&uinp, 0, sizeof(uinp));
    //设备的别名
    strncpy(uinp.name, "ShaoTouchScreen", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 4;
    uinp.id.bustype = BUS_USB;
    uinp.absmin[ABS_MT_SLOT] = 0;
    uinp.absmax[ABS_MT_SLOT] = 9; // MT代表multi touch 多指触摸 最大手指的数量我们设置9
    uinp.absmin[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmax[ABS_MT_TOUCH_MAJOR] = 15;
    uinp.absmin[ABS_MT_POSITION_X] = 0; // 屏幕最小的X尺寸
    uinp.absmax[ABS_MT_POSITION_X] = 1080; // 屏幕最大的X尺寸
    uinp.absmin[ABS_MT_POSITION_Y] = 0; // 屏幕最小的Y尺寸
    uinp.absmax[ABS_MT_POSITION_Y] = 1920; //屏幕最大的Y尺寸
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    uinp.absmax[ABS_MT_TRACKING_ID] = 65535;//按键码ID累计叠加最大值
    uinp.absmin[ABS_MT_PRESSURE] = 0;
    uinp.absmax[ABS_MT_PRESSURE] = 255;     //屏幕按下的压力值

    // Setup the uinput device
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);   //该设备支持按键
    ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);   //支持鼠标

    // Touch
    ioctl (uinp_fd, UI_SET_EVBIT,  EV_ABS); //支持触摸
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    ioctl (uinp_fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
    ioctl (uinp_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

    char str[20];
    memset(str,0,sizeof(str));
    sprintf(str,"%d",uinp_fd);

    //property_set("nvr_touch_screen_device",str);
    //ALOGD("nvr touch screen device strfd = %s , id = %d\n",str ,uinp_fd);

    /* Create input device into input sub-system */
    write(uinp_fd, &uinp, sizeof(uinp));
    ioctl(uinp_fd, UI_DEV_CREATE);
    return uinp_fd;

}
void inClose(){
    if (ioctl(fd_mouse, UI_DEV_DESTROY) < 0)
        die("error: ioctl");
    close(fd_mouse);
}

