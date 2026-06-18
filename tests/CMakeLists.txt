find_package(Catch2 3 REQUIRED)

add_executable(Tests)

set_target_properties(Tests 
    PROPERTIES 
        CXX_CLANG_TIDY ""
)

target_sources(Tests
    PRIVATE
        threadpool_tests.cpp
)

target_link_libraries(Tests 
    PRIVATE 
        Catch2::Catch2WithMain
        ThreadPool
)

include(Catch)
catch_discover_tests(Tests)