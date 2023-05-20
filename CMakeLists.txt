cmake_minimum_required(VERSION 3.6.0)
project(ecocar-telemetry-interface VERSION 0.1.0 LANGUAGES C CXX)

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

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/glfw/CMakeLists.txt")
    message(FATAL_ERROR "GLFW CMakeLists file not found.")
endif()

add_subdirectory(modules/glfw)
include_directories(modules/glfw/include)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/serial-com/CMakeLists.txt")
    message(FATAL_ERROR "Serial Com CMakeLists file not found.")
endif()

add_subdirectory(modules/serial-com)
include_directories(modules/serial-com)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/imgui/CMakeLists.txt")
    message(FATAL_ERROR "imgui CMakeLists file not found.")
endif()

add_subdirectory(modules/imgui)
include_directories(module/imgui)
include_directories(modules/imgui/backends)

add_subdirectory(modules/glad)
include_directories(modules/glad/include/glad)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(ecocar-telemetry-interface main.cpp)

# since we use glad we have to stop glfw from loading system gl.
add_compile_definitions(ecocar-telemetry-interface GLFW_INCLUDE_NONE)

target_link_libraries(ecocar-telemetry-interface PRIVATE glad)
target_link_libraries(ecocar-telemetry-interface PUBLIC imgui)
target_link_libraries(ecocar-telemetry-interface PUBLIC glfw)
target_link_libraries(ecocar-telemetry-interface PUBLIC SERIAL_COM)

# ok cmake is officially bad ass.
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
include(CPack)
