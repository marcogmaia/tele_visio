cmake_minimum_required(VERSION 3.15.0)
project(tele_visio VERSION 0.1.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(USER_CXX_FLAGS "-fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USER_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g -Wall -DDEBUG")

set(CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)

include_directories(lib/raylib/src)
add_subdirectory(lib/raylib)
include_directories(lib/imgui)
include(rlimgui)

include_directories(src)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} src/main.cpp)
# target_compile_features(${PROJECT_NAME} cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt raylib rlimgui implot)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
