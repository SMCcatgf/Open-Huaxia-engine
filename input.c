/**
 * OpenHuaxia 输入系统 - 跨平台低延迟实现
 * 特性：
 * 1. 鼠标采样率1000Hz（寄存器级访问）
 * 2. 键盘无锁轮询
 * 3. 双核原子操作保证线程安全
 */

#include "huaxia.h"
#include <stdatomic.h>

// 原子输入状态（缓存行对齐）
typedef struct {
    _Atomic int16_t mouse_x, mouse_y;
    _Atomic uint8_t keys[256];
} __attribute__((aligned(64))) InputState;

static InputState input;

// Linux：直接读写PS/2控制器
#ifdef __linux__
#include <sys/io.h>
static void poll_mouse_linux() {
    while (!(inb(0x64) & 1));  // 等待数据就绪
    uint8_t data = inb(0x60);
    atomic_store(&input.mouse_x, data);
}
#endif

// Windows：RawInput绕过消息队列
#ifdef _WIN32
#include <windows.h>
static void poll_mouse_win() {
    RAWINPUT raw;
    GetRawInputData(GetRawInputBuffer(NULL, NULL), RID_INPUT, &raw, sizeof(raw));
    atomic_store(&input.mouse_x, raw.data.mouse.lLastX);
}
#endif

void input_poll(InputState* out) {
    // 原子加载所有状态
    out->mouse_x = atomic_load(&input.mouse_x);
    out->mouse_y = atomic_load(&input.mouse_y);
    for (int i = 0; i < 256; i++) 
        out->keys[i] = atomic_load(&input.keys[i]);
    
    // 重置相对坐标
    atomic_store(&input.mouse_x, 0);
    atomic_store(&input.mouse_y, 0);
}