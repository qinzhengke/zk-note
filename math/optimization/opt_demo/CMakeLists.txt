cmake_minimum_required(VERSION 3.0)

project(opt_demo)
set(ZKNOTE_DIR "../../..")

find_package(Python2 COMPONENTS Development NumPy REQUIRED)

add_executable(opt_demo
    main.cpp
    )

target_include_directories(opt_demo PRIVATE
    ${Python2_INCLUDE_DIRS}
    ${Python2_NumPy_INCLUDE_DIRS}
    ${ZKNOTE_DIR}/3rd-party/matplotlib-cpp
    )
target_link_libraries(opt_demo Python2::Python Python2::NumPy)
target_compile_features(opt_demo PRIVATE cxx_std_11)
