cmake_minimum_required(VERSION 3.6.0)

project(telemetry-interface VERSION 0.1.0 LANGUAGES C CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
find_package(Git QUIET)

# https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/AEngine/CMakeLists.txt")
    message(FATAL_ERROR "AEngine CMakeLists file not found.")
endif()

set(AE_COMPILE_NO_TEST ON)

add_subdirectory(modules/AEngine)

target_compile_definitions(AEngine PUBLIC AE_PLATFORM_WINDOWS)

# i didnt want to fork anymore so i just added a external cmake file, if i need to edit the repo then i will fork
include(imgui-knobs.cmake)
include_directories(modules/imgui-knobs)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_executable(telemetry-interface ${SOURCES} ${HEADERS})
target_include_directories(telemetry-interface PUBLIC src)
target_link_libraries(telemetry-interface PUBLIC AEngine)
target_link_libraries(telemetry-interface PUBLIC IMGUI-KNOBS)

include_directories(modules/AEngine/AEngine/vendor/spdlog/include)
include_directories(modules/AEngine/AEngine/vendor)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT telemetry-interface)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

set_target_properties(AEngine PROPERTIES FOLDER "Dependencies")

#if(AE_PLATFORM_LINUX)
#target_link_libraries(AEngine PUBLIC X11)
#endif()
#target_compile_definitions(GLFW PUBLIC GLFW_BUILD_X11)
# if(NOT WIN32)
# target_link_libraries(ecocar-telemetry-interface PUBLIC SERIAL_COM)
# endif()

# target_link_libraries(ecocar-telemetry-interface PUBLIC X11)

# ok cmake is officially bad ass.