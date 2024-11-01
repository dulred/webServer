cmake_minimum_required(VERSION 3.10)

project(combined_project)

set(CMAKE_CXX_STANDARD 11)

# 设置包含目录
include_directories(
    D:/dulred/dev/3rdlibs/done/boost_1_86_0/include/boost-1_86
    D:/dulred/dev/3rdlibs/done/json-3.11.3/include
    D:/dulred/dev/source/c++/vscode/webServer/include
    D:/dulred/dev/3rdlibs/done/googletest-release-1.12.1/include
)
# 添加链接库路径
link_directories(D:/dulred/dev/3rdlibs/done/googletest-release-1.12.1/lib)

# 添加 http_server_async 的源文件
set(http_server_async_SRCS ./src/http_server_async.cpp ./src/utils/json_utils.cpp)
add_executable(http_server_async ${http_server_async_SRCS})
target_link_libraries(http_server_async ws2_32 mswsock)

# 添加 runTests 的源文件
set(runTests_SRCS ./test/unit/test_json_utils.cpp ./src/utils/json_utils.cpp)
add_executable(runTests ${runTests_SRCS})
target_link_libraries(runTests gtest gtest_main)