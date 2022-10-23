
cmake_minimum_required(VERSION 3.12)
project(cpp_app)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/exe)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ffast-math")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -ffast-math -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -ffast-math -DNDEBUG -s")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ffast-math -DNDEBUG -g")

include_directories(src)
include_directories(src/emu)
include_directories(src/scv)
include_directories(src/utils)

file(GLOB sourcefiles 
"src/*.h" "src/*.cpp"
"src/emu/*.h" "src/emu/*.cpp"
"src/utils/*.h" "src/utils/*.cpp"
"src/scv/*.h" "src/scv/*.cpp"
)
add_executable(${PROJECT_NAME} ${sourcefiles})

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
${OPENGL_LIBRARIES}
${GLEW_LIBRARIES}
${EXTRA_LIBS}
glfw SDL2_image SDL2_ttf SDL2 SDL2main)
