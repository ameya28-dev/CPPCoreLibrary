cmake_minimum_required(VERSION 3.28)
project(CoreLibrary)

set(CMAKE_CXX_STANDARD 17)

set(CoreLibrary_SOURCES
        src/random/random.cpp
        src/logger/manager/manager.cpp
        src/utils/utils.cpp
        src/http/types/types.cpp
        src/http/codes/http_status.cpp
        src/http/codes/network_status.cpp
        src/http/base_client/base_client.cpp
)

if (WIN32)
    list(APPEND CoreLibrary_SOURCES
            src/win_http/types/types.cpp
            src/win_http/handler/network.cpp
    )
endif ()

add_library(CoreLibrary STATIC ${CoreLibrary_SOURCES})

target_include_directories(CoreLibrary
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(CoreLibrary PRIVATE $<$<CONFIG:Debug>:PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}">)
set(TZDATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tzdata")
message(STATUS "absolute path of tzdata folder: ${TZDATA_DIR}")
if (EXISTS "${TZDATA_DIR}")
    file(GLOB_RECURSE RESOURCES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tzdata/*")
    add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/tzdata_copy_stamp"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tzdata" "$<TARGET_FILE_DIR:CoreLibrary>/tzdata"
            COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/tzdata_copy_stamp"
            DEPENDS ${RESOURCES_FILES}
            COMMENT "Copying tzdata to output directory"
    )
    add_custom_target(copy_resource_files DEPENDS "${CMAKE_BINARY_DIR}/tzdata_copy_stamp")
    add_dependencies(CoreLibrary copy_resource_files)
endif ()


set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CPM_DOWNLOAD_VERSION 0.42.1)

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake" ${CPM_DOWNLOAD_LOCATION}
    )
endif ()

message("${CPMstatus}")

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME fmt
        VERSION 12.1.0
        URL "https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS "FMT_TEST OFF" "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME spdlog
        VERSION 1.17.0
        URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS
        "SPDLOG_FMT_EXTERNAL ON"   # Tells spdlog to find and use your 'fmt' package
        "SPDLOG_BUILD_TESTS OFF"   # Disables internal spdlog tests
        "SPDLOG_BUILD_EXAMPLE OFF" # Skips spdlog's example folder
        "SPDLOG_BUILD_BENCH OFF"
        "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME nlohmann_json
        VERSION 3.12.0
        URL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz"
        URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS "JSON_BuildTests OFF"
)

target_link_libraries(CoreLibrary PUBLIC fmt::fmt)
target_link_libraries(CoreLibrary PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(CoreLibrary PUBLIC spdlog::spdlog)

# --- Testing Configuration ---
CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        VERSION 1.15.2
        OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt ON"
)

if (googletest_ADDED)
    enable_testing()

    add_executable(CoreTests
            tests/test_random.cpp
            tests/test_logger.cpp
            tests/test_winhttp.cpp
            tests/test_base_client.cpp
            tests/test_http_status.cpp
            tests/test_http_status.cpp
    )

    target_link_libraries(CoreTests PUBLIC CoreLibrary gtest_main)

    include(GoogleTest)
    gtest_discover_tests(CoreTests)
endif ()

if (WIN32)
    message("Linking WinHTTP on Windows")
    target_link_libraries(CoreLibrary PUBLIC winhttp.lib)
endif ()