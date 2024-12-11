enable_testing()

set(CTEST_OUTPUT_ON_FAILURE ON)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

file(GLOB_RECURSE TestFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/tests PREFIX "" FILES ${TestFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX "" FILES ${SourceFiles})

Include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
        GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)

add_executable(GTests ${TestFiles} ${SourceFiles})

target_compile_features(GTests PRIVATE cxx_std_17)

target_link_libraries(GTests PRIVATE gtest gtest_main gmock gmock_main)

target_include_directories(GTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_compile_definitions(GTests PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},COMPILE_DEFINITIONS>)

target_compile_definitions(GTests PUBLIC
        JUCE_MODAL_LOOPS_PERMITTED=1
)

target_link_libraries(GTests PRIVATE ProjectCode gtest gtest_main gmock gmock_main)

include(GoogleTest)
gtest_discover_tests(GTests)