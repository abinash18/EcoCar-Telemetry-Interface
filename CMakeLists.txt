cmake_minimum_required(VERSION 3.6.0)

project(telemetry-interface VERSION 0.1.0 LANGUAGES C CXX)

find_package(Git QUIET)

# https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" OFF)

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

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/modules/imgui/CMakeLists.txt")
    message(FATAL_ERROR "imgui CMakeLists file not found.")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(telemetry-interface)
target_sources(telemetry-interface PUBLIC ${SOURCES})
target_include_directories(telemetry-interface PUBLIC src)
target_link_libraries(telemetry-interface PUBLIC AEngine)

# target_compile_definitions(glfw PUBLIC GLFW_BUILD_WIN32)
# if(NOT WIN32)
# target_link_libraries(ecocar-telemetry-interface PUBLIC SERIAL_COM)
# endif()

# target_link_libraries(ecocar-telemetry-interface PUBLIC X11)

# ok cmake is officially bad ass.
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_BUILD_WIN32)
# include(CPack)
