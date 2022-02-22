以tinyply为例讲解现代cmake用法
========

tinyply是一个用于读取PLY点云文件的开源代码库，使用的是现代风格的cmake语法，本身规模较小，很适合学习。

先来看看tinyply目录下和cmake相关的文件结构

```
tinyply-2.3.2/
|-- CMakeLists.txt
|-- appveyor.yml
|-- cmake
|   |-- Config.cmake.in
|   `-- tinyply.pc.in
|-- readme.md
|-- source
|   |-- Makefile
|   |-- example-utils.hpp
|   |-- example.cpp
|   |-- tests.cpp
|   |-- tinyply.cpp
|   `-- tinyply.h
`-- third-party
    `-- doctest.h

```

先来看看CMakeLists.txt文件，注释就直接写在了代码里。

```cmake
# 设置cmake工具的最低版本，禁止用老版本cmake来构建当前工程。
cmake_minimum_required(VERSION 2.8)

# 设置当前工程的编程语言为C++，如果是纯CPU代码，这一行也可以省略。
project(tinyply LANGUAGES CXX)

# 设置变量，工程版本，后面会用到
set(PROJECT_VERSION 2.3)

include_directories("${CMAKE_SOURCE_DIR}/source")
include_directories("${CMAKE_SOURCE_DIR}/third-party")

set(CMAKE_DEBUG_POSTFIX "d")

# Library
set(SHARED_LIB true CACHE BOOL "Build as shared library")
if(${SHARED_LIB})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(tinyply SHARED source/tinyply.cpp source/tinyply.h)
else()
    add_library(tinyply STATIC source/tinyply.cpp source/tinyply.h)
endif()

set(BUILD_TESTS false CACHE BOOL "Build tests")

# Example Application
add_executable(example source/example.cpp)
target_link_libraries(example PRIVATE tinyply)

# Test Application
if(${BUILD_TESTS})
  add_executable(tests source/tests.cpp)
  target_link_libraries(tests PRIVATE tinyply)
endif()

# pkg-config support
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
if(UNIX)
  configure_file(cmake/tinyply.pc.in ${generated_dir}/tinyply.pc @ONLY)
  install(FILES "${generated_dir}/tinyply.pc" DESTINATION lib/pkgconfig)
endif()

# CMake package config support
include(CMakePackageConfigHelpers)

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "")

write_basic_package_version_file(
  "${version_config}"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  "cmake/Config.cmake.in"
  "${project_config}"
  INSTALL_DESTINATION "${config_install_dir}"
)

# Install
install(TARGETS tinyply EXPORT ${targets_export_name}
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(FILES source/tinyply.h
        DESTINATION include)

install(FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}")

install(EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}")
```