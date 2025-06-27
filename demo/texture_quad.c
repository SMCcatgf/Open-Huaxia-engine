/**
 * OpenHuaxia 示例：纹理四边形性能测试
 * 用途：评估4K纹理填充率
 */

#include "../huaxia.h"

// 4K纹理数据（RGBA8）
#define TEX_SIZE 4096
static uint32_t texture[TEX_SIZE * TEX_SIZE];

// 四边形顶点（位置+UV）
typedef struct {
    Vec3 pos;
    Vec2 uv;
} QuadVertex;

static const QuadVertex quad_verts[4] = {
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
};

// 生成棋盘格纹理
void generate_texture() {
    for (int y = 0; y < TEX_SIZE; y++) {
        for (int x = 0; x < TEX_SIZE; x++) {
            bool checker = (x / 32 + y / 32) % 2;
            texture[y * TEX_SIZE + x] = checker ? 0xFFFFFFFF : 0xFF000000;
        }
    }
}

int main() {
    // 生成测试纹理
    generate_texture();

    // 初始化引擎
    if (!render_init(HUAXIA_RENDER_AUTO) || 
        !render_upload_texture(texture, TEX_SIZE, TEX_SIZE)) {
        fprintf(stderr, "初始化失败\n");
        return 1;
    }

    // 性能测试循环
    uint64_t frames = 0;
    double start = get_time();
    while (frames < 60000) {  // 1分钟测试
        render_begin();
        render_draw_quads(quad_verts, 4);
        render_end();
        frames++;
    }

    // 输出结果
    double fps = frames / (get_time() - start);
    printf("4K纹理填充率: %.1f FPS\n", fps);
    return 0;
}