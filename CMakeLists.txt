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
        include/core/date/date.cpp
        include/core/initializer.hpp
)

if (WIN32)
    list(APPEND CoreLibrary_SOURCES
            src/win_http/types/types.cpp
            src/win_http/handler/network.cpp
    )
endif ()

add_library(CoreLibrary STATIC ${CoreLibrary_SOURCES})

if (WIN32)
    target_compile_definitions(CoreLibrary PUBLIC NOMINMAX)
endif ()

target_include_directories(CoreLibrary
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CORE_TZDATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tzdata" CACHE INTERNAL "")

function(core_deploy_tzdata TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CORE_TZDATA_PATH}"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/tzdata"
            COMMENT "Copying TimeZone database to output directory..."
            VERBATIM
    )
endfunction()

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
        NAME date
        VERSION 3.0.4
        URL "https://github.com/HowardHinnant/date/archive/refs/tags/v3.0.4.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        OPTIONS
        "ENABLE_DATE_TESTING OFF"
        "USE_SYSTEM_TZ_DB ON"
        "BUILD_TZ_LIB ON"
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
target_link_libraries(CoreLibrary PUBLIC date::date date::date-tz)

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