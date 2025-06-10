//
// Created by Administrator on 2025/6/10.
//

#ifndef PIKAPYTHON_H
#define PIKAPYTHON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PikaObj.h"

PikaObj *pikaPythonInit(void);

#ifdef __cplusplus
}
#endif

class pikaPython {
public:
    pikaPython() {
        if (++refCount == 1) {
            pikaMain = pikaPythonInit(); // 第一次构造时初始化
        }
    }

    ~pikaPython() {
        if (--refCount == 0) {
            if (pikaMain) {
                obj_deinit(pikaMain);
                pikaMain = nullptr;
            }
        }
    }

    // 禁止拷贝构造和赋值操作（防止浅拷贝）
    pikaPython(const pikaPython &) = delete;

    pikaPython &operator=(const pikaPython &) = delete;

    void run(char *script) {
        if (pikaMain && script) {
            printf("<<<<<<< pikaMain run >>>>>>>>\n");
            obj_run(pikaMain, script);
            printf("<<<<<<< pikaMain end >>>>>>>>\n");
        } else {
            printf("<<<<<<<pikaMain or script is null>>>>>>>>\n");
        }
    }

private:
    PikaObj *pikaMain; // 共享的 PikaObj 指针
    int refCount; // 引用计数
};


#endif //PIKAPYTHON_H
