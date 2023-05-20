cmake_minimum_required(VERSION 3.0.0)
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

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/serial-com/CMakeLists.txt")
    message(FATAL_ERROR "Serial Com CMakeLists file not found.")
endif()

add_subdirectory(modules/serial-com)

add_executable(ecocar-telemetry-interface main.cpp)

target_link_libraries(ecocar-telemetry-interface glfw)
target_link_libraries(ecocar-telemetry-interface serial-com)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
