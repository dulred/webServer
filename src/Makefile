# 声明虚拟目标
.PHONY: all clean
# 指定编译器
CXX = g++
CXXFLAGS = -std=c++11
INCLUDES = -I D:\dulred\dev\3rdlibs\done\boost_1_86_0\include\boost-1_86 \
           -I D:\dulred\dev\3rdlibs\done\json-3.11.3\include \
           -I D:\dulred\dev\source\c++\vscode\webServer\include
LIBS = -lws2_32 -lmswsock
# LIBDIRS = -L /path/to/gtest/lib
SRCS = http_server_async.cpp ./utils/json_utils.cpp
TARGET = http_server_async

# 编译所有源文件并生成目标文件
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LIBS)

# 清理生成的文件
clean:
	rm -f $(TARGET)
