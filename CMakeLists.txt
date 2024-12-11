cmake_minimum_required(VERSION 3.28.1)

set(PROJECT_NAME "NAM")
set(PLUGIN_VERSION 0.0.1)
project(${PROJECT_NAME} VERSION ${PLUGIN_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ASIO_HOME $ENV{ASIO_SDK_HOME})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(SYSTEM /usr/local/include)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_libraries(stdc++fs)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
else()
    message(FATAL_ERROR "Unrecognized Platform!")
endif()

if (CMAKE_PREFIX_PATH)
    find_package(JUCE CONFIG REQUIRED)
else()
    add_subdirectory(deps/JUCE)
endif()

add_subdirectory(deps/nam-core/Dependencies/eigen)
add_subdirectory(deps/json)

add_library(ProjectCode INTERFACE)

target_include_directories(ProjectCode INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-resampler"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/Dependencies/eigen"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/Dependencies/nlohmann"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/json/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nlohmann"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/"
        "${ASIO_HOME}/common"
)

target_compile_definitions(ProjectCode INTERFACE
        NAM_SAMPLE_FLOAT
        DSP_SAMPLE_FLOAT
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0
        PLUG_VERSION="${PLUGIN_VERSION}"
        JUCE_ASIO=1
)

target_sources(ProjectCode INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/activations.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/convnet.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/dsp.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/get_dsp.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/lstm.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/util.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/nam-core/NAM/wavenet.cpp"
)

target_link_libraries(ProjectCode INTERFACE
        BinaryData
        Eigen3::Eigen
        nlohmann_json::nlohmann_json
        juce::juce_audio_utils
        juce::juce_dsp
)

set(FORMATS Standalone VST3)
juce_add_plugin(${PROJECT_NAME}
        VERSION ${PLUGIN_VERSION}
        COMPANY_NAME "Joe-Loftus"
        PLUGIN_CODE NAM1
        FORMATS "${FORMATS}"
        PRODUCT_NAME "NAM JUCE"
        COPY_PLUGIN_AFTER_BUILD TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE ProjectCode)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            "$<$<CONFIG:DEBUG>:/W2>"
            "$<$<CONFIG:RELEASE>:/O2>"
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            "$<$<CONFIG:DEBUG>:-Og;-ggdb>"
            "$<$<CONFIG:RELWITHDEBINFO>:-Ofast;-march=x86-64-v3>"
            "$<$<CONFIG:RELEASE>:-Ofast;-march=x86-64-v3>"
    )
endif()


add_subdirectory(assets)

add_subdirectory(source)

include(GTests)
