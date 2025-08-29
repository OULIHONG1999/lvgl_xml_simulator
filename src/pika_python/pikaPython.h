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

#include <mutex>

#define PIKA_RUN(script) PikaPython::getInstance().run((char *) script)

class PikaPython {
public:
    // 获取全局唯一实例
    static PikaPython& getInstance() {
        static PikaPython instance; // C++11 静态局部变量线程安全
        return instance;
    }

    // 禁止拷贝和赋值
    PikaPython(const PikaPython&) = delete;
    PikaPython& operator=(const PikaPython&) = delete;

    void run(char *script) {
        if (pikaMain && script) {
            printf("<<<<<<< pikaMain run >>>>>>>>\n");
            obj_run(pikaMain, script);
            printf("<<<<<<< pikaMain end >>>>>>>>\n");
        } else {
            printf("<<<<<<< pikaMain or script is null >>>>>>>>\n");
        }
    }

private:
    PikaPython() {                 // 构造时初始化
        pikaMain = pikaPythonInit();
    }

    ~PikaPython() {                // 析构时释放
        if (pikaMain) {
            obj_deinit(pikaMain);
            pikaMain = nullptr;
        }
    }

    PikaObj *pikaMain = nullptr;
};

#endif // PIKAPYTHON_H
