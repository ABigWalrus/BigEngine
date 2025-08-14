set(TEST_SOURCES
    scene_test.cpp
)

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# include(CTest)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/Catch2/extras")
include(Catch)
catch_discover_tests(tests)
