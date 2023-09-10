cmake_minimum_required(VERSION 3.19)
project(main LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SRC main.cpp ./src/MainWindow.cpp ./src/CodeEditorWidget.cpp)
set(HDR ./include/MainWindow.hpp ./include/CodeEditorWidget.hpp)

add_executable(main ${SRC} ${HDR})

target_link_libraries(main PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets lexer)
target_include_directories(main PUBLIC include)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "D:\\qt\\5.12.12\\mingw73_64\\")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(lib)