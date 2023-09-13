set(TARGET_NAME test)

include_directories(../)

set(HDR include/unit_test.hpp include/unit_test_lexer.hpp include/unit_test_parser.hpp)
set(SRC main.cpp)

add_executable(test ${SRC} ${HDR})

target_link_libraries(${TARGET_NAME} PRIVATE lexer parser)