cmake_minimum_required(VERSION 3.10)

project(cli-text-editor)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")

link_directories(${PROJECT_SOURCE_DIR}/contrib/curses/lib)

include_directories(editor PUBLIC ${PROJECT_SOURCE_DIR}/contrib/curses/include)

add_executable(editor main.c)

target_link_libraries(editor PUBLIC ncursesw_g)

add_definitions(-DNCURSES_WIDECHAR -DCMAKE_BUILD_TYPE=Debug)
