
file(GLOB cli_SRC "*.cpp")

add_executable(brim ${cli_SRC})
target_link_libraries(brim brim_lib)