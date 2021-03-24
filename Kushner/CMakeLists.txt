project(Kushner)

add_library(Kushner Kushner.cpp Kushner.h)

target_include_directories(Kushner PUBLIC .)

target_link_libraries(Kushner Base)


