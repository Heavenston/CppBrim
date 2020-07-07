
file(GLOB brim_lib_SRC "*.cpp" "*.h")
add_library(brim_lib SHARED ${brim_lib_SRC})
target_include_directories(brim_lib PUBLIC .)
