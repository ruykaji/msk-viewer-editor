set(TARGET_NAME lexer)

file(GLOB_RECURSE HDR *.hpp)
file(GLOB_RECURSE SRC *.cpp)

add_library(${TARGET_NAME} ${HDR} ${SRC})

target_include_directories(${TARGET_NAME} PUBLIC include)