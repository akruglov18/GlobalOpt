project(Piyavsky)

add_library(Piyavsky Piyavsky.cpp Piyavsky.h)

target_include_directories(Piyavsky PUBLIC .)

target_link_libraries(Piyavsky Base)


