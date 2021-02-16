project(AGP)

add_library(AGP AGP.cpp AGP.h)

target_include_directories(AGP PUBLIC .)

target_link_libraries(AGP Base)


