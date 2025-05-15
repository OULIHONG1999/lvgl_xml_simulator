#include "page_manager.h"
#include "stdio.h"
//#include "remote_view/page_manager/page_manager.h"
/*
 * 使用方法
 * 1、调用init_activity_manager()，初始化管理器
 * 2、在头文件中添加user_activity_t结构体，方便在其他ui中进行调用
 * 3、在各自的ui文件中，在需要跳转页面的地方调用start_activity()，传入目标ui的结构体指针
 * 4、如果需要执行返回操作，需要调用stop_activity()，传入当前ui的结构体指针
 *
 * 在activity文件中，存放需要使用的界面以及控件，在文件中，需要创建user_activity_t结构体
 * 并且需要创建函数：void create_activity(void)，void destroy_activity(void)，用于对外的接口
 * */

#define TIMER lv_timer_t
#define OBJ lv_obj_t
#define SYS_BG_COLOR get_sys_background_colour()

activity_manager_t act_m;
bool isBooted = false;

void set_boot_state(bool state) {
    isBooted = state;
}

bool get_boot_state() {
    return isBooted;
}

lv_obj_t *create_activity_obj() {
    lv_obj_t *obj = lv_obj_create(NULL);
    lv_obj_set_size(obj, 320, 170);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
    return obj;
}

bool start_activity(user_activity_t *activity, void *param) {
    if (activity == NULL) {
        return false;
    }

    if (act_m.index >= MAX_ACTIVITY_NUM - 1) {
        // 检查是否还有空间
        return false;
    }

    // 如果当前为第一个活动，那么就不自增，移动下标
    act_m.index++; // 先递增索引
    //将需要创建的活动保存到列表中，并创建活动
    act_m.activity[act_m.index] = activity;
    act_m.activity[act_m.index]->param = param; //创建活动需要传入的数据
    bool ret = act_m.activity[act_m.index]->create_activity(); // 启动活动
    if (ret == false) {
        act_m.activity[act_m.index]->param = NULL;
        act_m.activity[act_m.index] = NULL;
        act_m.index--;
        return false;
    }

    // 执行前一个活动的销毁操作，如：删除空间、组、其他释放操作等
    if (act_m.index > 1) {
        // 检查前一个活动是否存在
        if (act_m.activity[act_m.index - 1]->destroy_activity != NULL) {
            act_m.activity[act_m.index - 1]->destroy_activity(0);
        }
    }
    return true;
}

void stop_activity() {
    // 判断是否有上一个活动
    if (act_m.index > 0) {
        for (int i = 0; i < MAX_ACTIVITY_NUM; i++) {
            // 是否有上一个活动
            if (act_m.activity[act_m.index - 1]->create_activity != NULL) {
                act_m.activity[act_m.index - 1]->create_activity();
                break;
            }
            if (i == MAX_ACTIVITY_NUM - 1) {
                // 如果没有上一个活动，那么就直接退出
                return;
            }
            act_m.index--;
        }

        // 释放当前活动
        if (act_m.activity[act_m.index]->destroy_activity != NULL) {
            act_m.activity[act_m.index]->destroy_activity(1);

            if (act_m.activity[act_m.index]->activity_obj != NULL) {
                lv_obj_del_delayed(act_m.activity[act_m.index]->activity_obj, 5);

                act_m.activity[act_m.index]->activity_obj = NULL;
            }
        }

        // 减少活动索引
        act_m.index--;
    }
}


void kill_self_activity(user_activity_t *act) {
    if (lv_scr_act() == act->activity_obj) {
        stop_activity();
        return;
    }

    // 释放当前活动
    for (int i = 0; i < MAX_ACTIVITY_NUM; i++) {
        if (act_m.activity[i] == act) {
            if (act_m.activity[i]->destroy_activity != NULL) {
                act_m.activity[i]->destroy_activity(1);
                if (act_m.activity[i]->activity_obj != NULL) {
                    lv_obj_del_delayed(act_m.activity[i]->activity_obj, 10);
                    act_m.activity[i]->activity_obj = NULL;
                }
            }
            act_m.activity[i] = NULL;
            act_m.index--;
        }
    }
}

///
/// @param error_msg 错误信息：
/// 1、当有错误信息的时候，跳转至错误信息页面，直到重启设备。
/// 2、当传入的信息为空的时候，递减页面管理器的index，并不跳转错误信息页面
void start_activity_fail(const char *error_msg) {
    // 释放当前活动
    act_m.index--;
    if (error_msg != NULL) {
        // 显示错误页面
        printf("[error_msg]: %s\n", error_msg);
    } else {
    }
}

char get_activity_index() {
    return act_m.index;
}
