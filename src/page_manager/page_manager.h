//
// Created by Administrator on 24-7-9.
//

#ifndef LVGL_PAGE_MANAGER_H
#define LVGL_PAGE_MANAGER_H


#include "lvgl/lvgl.h"
#define TIMEOUT (2*60)
//#define TIMEOUT (30)
#define ALARM_TIMEOUT (2*60)
#define SLEEP_TIMEOUT (2*60)
#define TIMER lv_timer_t
#define OBJ lv_obj_t
#define SYS_BG_COLOR get_sys_background_colour()

extern bool isBooted;

void set_boot_state(bool state);

bool get_boot_state();


typedef struct {
    lv_obj_t *activity_obj;

    bool (*create_activity)();

    void (*destroy_activity)(unsigned char);

    void *param;
} user_activity_t;

#define MAX_ACTIVITY_NUM 10

typedef struct {
    unsigned char index;
    user_activity_t *activity[MAX_ACTIVITY_NUM]; // 指针数组
} activity_manager_t;


typedef struct {
    lv_timer_t *timer;
    long cnt;
    long timeout;
} PageTimer;

static void reset_page_timer_cnt(PageTimer *timer, long cnt) {
    if (timer == NULL) return;
    if (timer->timer == NULL) return;
    lv_timer_reset(timer->timer);
    lv_timer_resume(timer->timer);
    timer->cnt = cnt;
}

static void set_page_timer_timeout(PageTimer *timer, long timeout) {
    if (timer == NULL) return;
    if (timer->timer == NULL) return;
    timer->timeout = timeout;
}

static void destroy_page_timer(PageTimer *timer) {
    if (timer == NULL) return;
    if (timer->timer != NULL) {
        // 暂停定时器
        lv_timer_pause(timer->timer);
        lv_timer_del(timer->timer);
        timer->timer = NULL;
        timer->cnt = 0;
        timer->timeout = 100000;
    }
}


extern user_activity_t act_home;


lv_obj_t *create_activity_obj();

bool start_activity(user_activity_t *activity, void *param);

void kill_self_activity(user_activity_t *act);

void stop_activity();

char get_activity_index();

void start_activity_fail(const char *error_msg);

#endif //LVGL_PAGE_MANAGER_H
