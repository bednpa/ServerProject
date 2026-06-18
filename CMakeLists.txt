cmake_minimum_required(VERSION 3.21)

project(ServerProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall 
    -Wextra 
    -Werror 
    -Wpedantic 
    -Wshadow 
    -Wconversion 
    -Wsign-conversion
)

find_program(CLANG_FORMAT_EXE clang-format)
file(GLOB_RECURSE FORMAT_SOURCES "*.cpp" "*.h" "*.tpp")
list(FILTER FORMAT_SOURCES EXCLUDE REGEX "${CMAKE_BINARY_DIR}")
add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
    COMMENT "Formatting all .cpp, .h, and .tpp files..."
)

option(CLANG_TIDY "Build with Clang-tidy" ON)

if(CLANG_TIDY)
    message(STATUS "Building with Clang-tidy.")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
    message(STATUS "Skipping Clang-tidy.")
endif()

option(BUILD_TESTS "Build test" ON)

if(BUILD_TESTS)
    message(STATUS "Building with tests enabled.")
    include(CTest)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests.")
endif()

add_subdirectory(threadpool)
add_subdirectory(tcpserver)
add_subdirectory(game)

add_executable(ServerProject)

target_sources(ServerProject
    PRIVATE
        main.cpp
)

target_link_libraries(ServerProject 
    PRIVATE 
        TcpServer
)