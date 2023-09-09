cmake_minimum_required(VERSION 3.6.0)

add_library(IMGUI-KNOBS STATIC modules/imgui-knobs/imgui-knobs.cpp modules/imgui-knobs/imgui-knobs.h)
set_target_properties(IMGUI-KNOBS PROPERTIES FOLDER "Dependencies")
target_link_libraries(IMGUI-KNOBS PRIVATE IMGUI)