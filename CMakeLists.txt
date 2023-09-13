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

target_link_libraries(main PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets lexer parser)
target_include_directories(main PUBLIC include)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(lib)