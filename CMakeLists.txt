cmake_minimum_required(VERSION 3.13)
project(CluedoBot)

set(CMAKE_CXX_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(CluedoBot main.cpp UI.cpp UI.h CluedoCard.cpp CluedoCard.h CluedoProgress.cpp CluedoProgress.h CluedoEntry.cpp CluedoEntry.h Player.cpp Player.h)
target_link_libraries(CluedoBot ${CURSES_LIBRARIES})