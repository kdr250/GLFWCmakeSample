cmake_minimum_required(VERSION 3.19)

project(GlfwWithCMake VERSION 1.0)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)
FetchContent_MakeAvailable(GLFW glew)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(
    GlfwWithCMake
    ${SOURCES}
)
target_link_libraries(
    GlfwWithCMake
    glfw
    libglew_static
)

if(APPLE)
    target_link_libraries(GlfwWithCMake "-framework OpenGL")
endif()

add_custom_command(
    TARGET GlfwWithCMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:GlfwWithCMake>/resources
)
