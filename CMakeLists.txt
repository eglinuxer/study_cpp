cmake_minimum_required(VERSION 3.10...3.21)

# ---------------------------------------------------------------------------------------
# Start study_cpp project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

study_cpp_extract_version()

project(study_cpp VERSION ${STUDY_CPP_VERSION} LANGUAGES CXX)
message(STATUS "Build study_cpp: ${STUDY_CPP_VERSION}")

include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# make sure __cplusplus is defined when using msvc and enable parallel build
if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

# ---------------------------------------------------------------------------------------
# Set STUDY_CPP_MASTER_PROJECT to ON if we are building study_cpp
# ---------------------------------------------------------------------------------------
# Check if study_cpp is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED STUDY_CPP_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(STUDY_CPP_MASTER_PROJECT ON)
    else()
        set(STUDY_CPP_MASTER_PROJECT OFF)
    endif()
endif()

option(STUDY_CPP_BUILD_ALL "Build all artifacts" OFF)

# build shared option
option(STUDY_CPP_BUILD_SHARED "Build shared library" OFF)

# example options
option(STUDY_CPP_BUILD_EXAMPLE "Build example" ${STUDY_CPP_MASTER_PROJECT})

# testing options
option(STUDY_CPP_BUILD_TESTS "Build tests" OFF)

# bench options
option(STUDY_CPP_BUILD_BENCH "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)" OFF)

# sanitizer options
option(STUDY_CPP_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)

# warning options
option(STUDY_CPP_BUILD_WARNINGS "Enable compiler warnings" OFF)

# install options
option(STUDY_CPP_INSTALL "Generate the install target" ${STUDY_CPP_MASTER_PROJECT})
option(STUDY_CPP_NO_EXCEPTIONS "Compile with -fno-exceptions. Call abort() on any study_cpp exceptions" OFF)

add_subdirectory(3rd)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    option(STUDY_CPP_CLOCK_COARSE "Use CLOCK_REALTIME_COARSE instead of the regular clock," OFF)
else()
    set(STUDY_CPP_CLOCK_COARSE OFF CACHE BOOL "non supported option" FORCE)
endif()

find_package(Threads REQUIRED)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

set(STUDY_CPP_SRCS src/main.cpp)

if(STUDY_CPP_BUILD_SHARED OR BUILD_SHARED_LIBS)
    if(WIN32)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
        list(APPEND STUDY_CPP_SRCS ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
    endif()
    add_library(study_cpp SHARED ${STUDY_CPP_SRCS} ${STUDY_CPP_ALL_HEADERS})
    target_compile_definitions(study_cpp PUBLIC STUDY_CPP_SHARED_LIB)
    if(MSVC)
        target_compile_options(study_cpp PUBLIC $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<COMPILE_LANGUAGE:CUDA>>>:/wd4251
                                             /wd4275>)
    endif()
else()
    # add_library(study_cpp STATIC ${STUDY_CPP_SRCS} ${STUDY_CPP_ALL_HEADERS})
    add_executable(study_cpp ${STUDY_CPP_SRCS})
endif()

# add_library(eglinux::study_cpp ALIAS study_cpp)

target_compile_definitions(study_cpp PUBLIC STUDY_CPP_COMPILED_LIB)
target_include_directories(study_cpp PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
# target_link_libraries(study_cpp PUBLIC Threads::Threads)
target_link_libraries(study_cpp PRIVATE spdlog::spdlog_header_only)
target_link_libraries(study_cpp PRIVATE nlohmann_json::nlohmann_json)
study_cpp_enable_warnings(study_cpp)

set_target_properties(study_cpp PROPERTIES VERSION ${STUDY_CPP_VERSION} SOVERSION ${STUDY_CPP_VERSION_MAJOR})
set_target_properties(study_cpp PROPERTIES DEBUG_POSTFIX _debug)

if(STUDY_CPP_NO_EXCEPTIONS AND NOT MSVC)
    target_compile_options(study_cpp PRIVATE -fno-exceptions)
endif()

# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
if(STUDY_CPP_BUILD_EXAMPLE OR STUDY_CPP_BUILD_ALL)
    message(STATUS "Generating example(s)")
    add_subdirectory(example)
    study_cpp_enable_warnings(example)
endif()

if(STUDY_CPP_BUILD_TESTS OR STUDY_CPP_BUILD_ALL)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(tests)
endif()

if(STUDY_CPP_BUILD_BENCH OR STUDY_CPP_BUILD_ALL)
    message(STATUS "Generating benchmarks")
    add_subdirectory(bench)
endif()

# ---------------------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build_3rd)
if(STUDY_CPP_INSTALL)
    message(STATUS "Generating install")
    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/studyCppConfig.cmake.in")
    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/studyCppConfig.cmake")
    set(config_targets_file "studyCppConfigTargets.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/studyCppConfigVersion.cmake")
    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/study_cpp")
    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")

    # ---------------------------------------------------------------------------------------
    # Include files
    # ---------------------------------------------------------------------------------------
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(
        TARGETS study_cpp
        EXPORT study_cpp
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # ---------------------------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------------------------
    get_target_property(PKG_CONFIG_DEFINES study_cpp INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE ";" " -D" PKG_CONFIG_DEFINES "${PKG_CONFIG_DEFINES}")
    string(CONCAT PKG_CONFIG_DEFINES "-D" "${PKG_CONFIG_DEFINES}")
    configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------------------------
    install(EXPORT study_cpp DESTINATION ${export_dest_dir} NAMESPACE study_cpp:: FILE ${config_targets_file})

    include(CMakePackageConfigHelpers)
    configure_package_config_file("${project_config_in}" "${project_config_out}"
        INSTALL_DESTINATION ${export_dest_dir})

    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

    # ---------------------------------------------------------------------------------------
    # Support creation of installable packages
    # ---------------------------------------------------------------------------------------
    include(cmake/studyCppCPack.cmake)
endif()
