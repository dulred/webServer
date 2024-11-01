cmake_minimum_required(VERSION 3.10)

project(http_server_async)

set(CMAKE_CXX_STANDARD 11)

# 设置包含目录
include_directories(
    D:/dulred/dev/3rdlibs/done/boost_1_86_0/include/boost-1_86
    D:/dulred/dev/3rdlibs/done/json-3.11.3/include
    D:/dulred/dev/source/c++/vscode/webServer/include
)

# 添加源文件
set(SRCS http_server_async.cpp ./utils/json_utils.cpp)


add_executable(http_server_async ${SRCS})

# 添加要链接的库
target_link_libraries(http_server_async ws2_32 mswsock)
