# Define the environment for cross-compiling with 32-bit MinGW-w64 GCC
set(CMAKE_SYSTEM_NAME Windows) # Target system name
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")

# SET(CMAKE_RC_COMPILER "x86_64-w64-mingw32-windres")
# SET(CMAKE_RANLIB "x86_64-w64-mingw32-ranlib")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Configure the behaviour of the find commands
set(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")


#fuckkkckkckckckkckc
# why bill why did u make this so hard
