/**
 * OpenHuaxia —— 主入口文件
 * 特性：
 * 1. 双核任务分配：主线程逻辑，渲染线程提交
 * 2. 自适应帧率控制
 * 3. 崩溃时自动保存状态
 */

#include "huaxia.h"
#include <stdio.h>
#include <stdlib.h>

// 全局状态
static struct {
    RenderBackend backend;
    bool running;
    InputState input;
} engine;

// 渲染线程函数（绑定到CPU1）
static void* render_thread(void* arg) {
    (void)arg;
    thread_bind_core(1); // 绑核优化

    while (engine.running) {
        render_frame();
    }
    return NULL;
}

// 初始化引擎
static bool engine_init() {
    // 自动选择渲染后端
    engine.backend = 
#ifdef _WIN32
        HUAXIA_RENDER_DX12;
#else
        HUAXIA_RENDER_VULKAN;
#endif

    if (!render_init(engine.backend)) {
        fprintf(stderr, "渲染初始化失败！\n");
        return false;
    }

    // 绑定主线程到CPU0
    thread_bind_core(0);
    return true;
}

// 主循环
static void engine_loop() {
    uint64_t frame_count = 0;
    engine.running = true;

    pthread_t thread;
    pthread_create(&thread, NULL, render_thread, NULL);

    while (engine.running) {
        // 输入处理（非阻塞）
        input_poll(&engine.input);

        // 逻辑更新（示例：移动三角形）
        if (engine.input.keys['W']) {
            // TODO: 相机移动
        }

        frame_count++;
    }

    pthread_join(thread, NULL);
}

int main(int argc, char** argv) {
    printf("OpenHuaxia v%s\n", HUAXIA_VERSION);

    if (!engine_init()) {
        return EXIT_FAILURE;
    }

    engine_loop();
    return EXIT_SUCCESS;
}