/**
 * OpenHuaxia —— 华夏引擎开源版核心头文件
 * 设计原则：
 * 1. 零依赖（仅需C99标准库）
 * 2. 极致性能（强制内联/SIMD）
 * 3. 双核CPU特化优化
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

// ==================== 全局配置 ====================
#define HUAXIA_VERSION "1.0-alpha"  // 当前版本
#define HUAXIA_MAX_THREADS 2        // 双核优化
#define HUAXIA_FRAME_MEMORY (16 * 1024 * 1024) // 每帧16MB静态内存池

// SIMD加速开关（x86/ARM自适应）
#if defined(__AVX2__)
    #define HUAXIA_USE_AVX2 1
    #include <immintrin.h>
#elif defined(__ARM_NEON)
    #define HUAXIA_USE_NEON 1
    #include <arm_neon.h>
#endif

// ==================== 数学库 ====================
typedef struct { float x, y, z; } Vec3;
typedef struct { float m[4][4]; } Mat4;

// 向量运算（强制内联）
static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}

// 矩阵乘法（AVX2/NEON加速）
Mat4 mat4_mul(Mat4 a, Mat4 b);

// ==================== 渲染系统 ====================
typedef enum {
    HUAXIA_RENDER_VULKAN,
    HUAXIA_RENDER_DX12,
    HUAXIA_RENDER_OPENGL // 兼容模式
} RenderBackend;

// 初始化渲染器（自动检测最优后端）
bool render_init(RenderBackend backend);

// 提交一帧（零缓冲策略）
void render_frame();

// ==================== 输入系统 ====================
typedef struct {
    int16_t mouse_x, mouse_y;
    uint8_t keys[256]; // 键盘状态
} InputState;

// 轮询输入（非阻塞）
void input_poll(InputState* state);

// ==================== 线程控制 ====================
// 绑定当前线程到指定核心（Linux/Windows）
void thread_bind_core(int core_id);