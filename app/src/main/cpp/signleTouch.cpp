#include <jni.h>
#include <stdint.h>
#include <cutils/log.h>
#include <linux/uinput.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cutils/properties.h>
#include <signleTouch.h>

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2

#define API_EXPORT __attribute__((visibility("default")))

static int global_tracking_id = 1;

namespace shao {

    extern long jptr(void *native_dtr) {
        return reinterpret_cast<intptr_t>(native_dtr);
    };


    extern void *native(long ptr) {
        return reinterpret_cast<void *>(ptr);
    };

}

//using namespace nvr;

extern "C" {
static bool device_writeEvent(int fd, uint16_t type, uint16_t keycode, int32_t value) {
    struct input_event ev;

    memset(&ev, 0, sizeof(struct input_event));

    ev.type = type;
    ev.code = keycode;
    ev.value = value;
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {
        char *mesg = strerror(errno);
        //ALOGD("nibiru uinput errormag info :%s\n",mesg);
        return false;
    }

    return true;
}

static void execute_sleep(int duration_msec) {
    usleep(duration_msec * 1000);
}

//startX startY 代表触摸down的坐标 endX 和 endY代表Up的坐标
//如果startX = startY 同时  endX = endY ，没有actionMove事件产生只有actionDown和actionUp事件
API_EXPORT void nvr_execute_touch(int fd, int startX, int startY, int endX, int endY) {
    //actionDown事件
    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, global_tracking_id++);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, startX);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, startY);
    device_writeEvent(fd, EV_ABS, ABS_MT_PRESSURE, 60);
    device_writeEvent(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //action_move事件
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, endX);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, endY);
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //action_up事件
    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    //事件发送完毕需要sync
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);
    //ALOGD(" one touch operation send end");
}

API_EXPORT void nvr_execute_down(int fd, int startX, int startY) {
    //actionDown事件
    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, global_tracking_id++);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, startX);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, startY);
    device_writeEvent(fd, EV_ABS, ABS_MT_PRESSURE, 60);
    device_writeEvent(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //事件发送完毕需要sync
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);
    //ALOGD(" one touch operation send end");
}

API_EXPORT void nvr_execute_move(int fd, int endX, int endY) {
    //action_move事件
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, endX);
    device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, endY);
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //事件发送完毕需要sync
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);
    //ALOGD(" one touch operation send end");
}

API_EXPORT void nvr_execute_up(int fd) {
    //action_up事件
    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);

    //事件发送完毕需要sync
    device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);
    //ALOGD(" one touch operation send end");
}


API_EXPORT void sendScreenTouch(int fd, int startX, int startY, int endX, int endY) {
    char package_status[PROPERTY_VALUE_MAX] = {0};
    // property_get("touch_screen_device",package_status,NULL);
    // int fd =  atoi(package_status);
    //ALOGD("touch screen fd = %d\n",fd);
    nvr_execute_touch(fd, startX, startY, endX, endY);
    execute_sleep(20);
}

}  
