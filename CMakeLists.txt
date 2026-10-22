cmake_minimum_required(VERSION 4.3)
project(
    chip8Emulator 
    VERSION 1.0
    LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

# To create a build for testing the CHIP-8 functions:
# - Create a build directory or delete CMakeCache.txt
# - Navigate to the build directory
# - Run "cmake -DCHIP8_TEST=ON .."
option(CHIP8_TEST "Active it to build the unit tests" OFF)

include(FetchContent)   


if (CHIP8_TEST)
    set(CUNIT_VERSION 3.5.5)
    FetchContent_Declare(
        cunit
        URL https://gitlab.com/cunity/cunit/-/archive/${CUNIT_VERSION}/cunit-${CUNIT_VERSION}.tar.gz
        FIND_PACKAGE_ARGS ${CUNIT_VERSION} EXACT
    )
    FetchContent_MakeAvailable(cunit)
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

set(SRC_FILES
    src/chip8_decoder.c
    src/chip8_emulator.c
    src/chip8_executor.c
    src/chip8_interface.c
    src/chip8_core.c
)

set(LIB_NAME "chip8Lib")

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_include_directories(${LIB_NAME} PUBLIC include)

target_link_libraries(${LIB_NAME} PUBLIC raylib)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


if (NOT CHIP8_TEST)
    add_executable(${PROJECT_NAME} src/main.c)
    target_compile_options(${PROJECT_NAME} PRIVATE "-lm")
else()
    add_executable(${PROJECT_NAME} tests/tests_main.c)
    target_compile_options(${PROJECT_NAME} PRIVATE "-lm")
    target_link_libraries(${PROJECT_NAME} PUBLIC cunit)
endif()

if(TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_NAME})