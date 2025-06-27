# OpenHuaxia 终极编译脚本
# 支持: Linux(gcc)/Windows(MinGW)/macOS(clang)

# 编译器配置
CC = gcc
CFLAGS = -O3 -march=native -std=c99 -Wall -Wextra
LDFLAGS = -lvulkan -lpthread

# Windows特殊处理
ifeq ($(OS), Windows_NT)
	CC = x86_64-w64-mingw32-gcc
	LDFLAGS = -L"./lib" -lvulkan-1 -lpthread
	EXE = .exe
else
	EXE =
endif

# 核心目标
TARGET = OpenHuaxia$(EXE)
SRCS = main.c render.c input.c math.c
OBJS = $(SRCS:.c=.o)

# 默认构建
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 示例Demo构建
demo:
	$(CC) $(CFLAGS) demo/triangle.c -o demo/triangle $(LDFLAGS)
	$(CC) $(CFLAGS) demo/texture_quad.c -o demo/texture_quad $(LDFLAGS)

# 清理
clean:
	rm -f $(OBJS) $(TARGET) demo/*.exe demo/*.o

# 跨平台运行指令
run:
ifeq ($(OS), Windows_NT)
	./$(TARGET) --dx12
else
	./$(TARGET) --vulkan
endif

.PHONY: all demo clean run