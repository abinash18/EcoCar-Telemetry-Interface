# https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ
# https://huawei-noah.github.io/bolt/docs/IOS_USAGE.html
set(tools ${PROJECT_SOURCE_DIR}/modules/osxcross/target/bin)
set(CMAKE_SYSTEM_NAME Darwin)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER ${tools}/o64-clang)
set(CMAKE_CXX_COMPILER ${tools}/o64-clang++)

set(CMAKE_C_COMPILER_AR ${tools}/x86_64-apple-darwin20.2-ar)
set(CMAKE_CXX_COMPILER_AR ${tools}/x86_64-apple-darwin20.2-ar)
set(CMAKE_LINKER ${tools}/x86_64-apple-darwin20.2-ld)
set(ENV{OSXCROSS_MP_INC} 1)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

# set(CMAKE_EXE_LINKER_FLAGS "--static -archx86_64")

# # Define the environment for cross-compiling with 32-bit MinGW-w64 GCC
# SET(CMAKE_SYSTEM_NAME Apple) # Target system name
# SET(CMAKE_SYSTEM_VERSION 1)

# SET(CMAKE_C_COMPILER ${tools}/o64-clang)
# SET(CMAKE_CXX_COMPILER ${tools}/o64-clang)

# # SET(CMAKE_RC_COMPILER "i686-w64-mingw32-windres")
# # SET(CMAKE_RANLIB "i686-w64-mingw32-ranlib")

# # Configure the behaviour of the find commands
# SET(CMAKE_FIND_ROOT_PATH "/usr/i686-w64-mingw32")
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)