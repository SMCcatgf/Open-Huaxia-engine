/**
 * OpenHuaxia 示例：最低延迟三角形渲染
 * 用途：测试渲染管线基础性能
 */

#include "../huaxia.h"
#include <math.h>

// 顶点数据（预对齐到64字节）
typedef struct __attribute__((aligned(64))) {
    Vec3 pos;
    Vec3 color;
} Vertex;

static const Vertex vertices[3] = {
    {{ 0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // 红
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // 绿
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}   // 蓝
};

int main() {
    // 初始化引擎（自动选择后端）
    if (!render_init(HUAXIA_RENDER_AUTO)) {
        fprintf(stderr, "渲染初始化失败\n");
        return 1;
    }

    // 主循环
    uint64_t frames = 0;
    double start_time = get_time();
    while (frames < 1000000) {  // 渲染100万帧测试
        // 旋转三角形（测试CPU负载）
        float angle = frames * 0.01f;
        Mat4 rot = mat4_rotation_z(angle);

        // 提交绘制命令
        render_begin();
        render_draw_triangles(vertices, 3, rot);
        render_end();

        frames++;
    }

    // 性能统计
    double elapsed = get_time() - start_time;
    printf("FPS: %.1f\n", frames / elapsed);
    return 0;
}