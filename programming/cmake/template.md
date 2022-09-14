# CMakeLists.txt模板

```cmake
# 现代cmake版本
cmake_minimum_required(VERSION 3.12)

# 使用文件夹名称作为project名称
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

# 使用cmake定义版本号
set(MAJOR_VER 1)
set(MINOR_VER 0)
set(PATCH_VER 0)
set(FULL_VERSION "${MAJOR_VER}.${MINOR_VER}.${PATCH_VER}")

# 设置默认版本为debug
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
message("=> cmake ${PROJECT_NAME} in Debug mode")
add_compile_options(-O0 -g -ggdb)
else()
message("=> cmake ${PROJECT_NAME} in Release mode")
endif()

# 构建example开关
option(BUILD_WITH_EXAMPLE ON)
if(BUILD_WITH_EXAMPLE)
# 构建example子目录
add_subdirectory(3rd-party/module_a)
endif()

# 编译某个目录总的所有源文件
file(GLOB my_SRC
    "src/*.cc"
)

# 新增目标静态lib，小工程，项目名称即为目标名称
add_library(${PROJECT_NAME}
    ${my_SRC}
)

# 目标包含路径，现代写法
target_include_directories(${PROJECT_NAME} PUBLIC
    3rd-party/module_a/include
)

# 目标链接库
target_link_libraries(${PROJECT_NAME}
    module_a
)

# 目标编译宏定义，现代写法
target_compile_definitions(${PROJECT_NAME} PUBLIC SOME_MACRO=1)

# 目标编译的C++标准版本
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

# 使用cmake定义版本号
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${FULL_VERSION})

```