cmake_minimum_required(VERSION 3.21)
# CMake Packages
include(FetchContent)
include(CMakePrintHelpers)

########################################################################################################################
################################## CMake Project Configuration #########################################################
########################################################################################################################

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)


# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE TRUE CACHE BOOL "Enable 'developer mode'")

# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT FALSE)

# Add cmake project_options
# https://github.com/cpp-best-practices/project_options
message(STATUS "Fetching cmake project options...")
FetchContent_Declare(_project_options
        GIT_REPOSITORY https://github.com/cpp-best-practices/project_options
        GIT_TAG origin/main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

project(
        matrix_comparison
        VERSION 1.0.1.0
        DESCRIPTION "Biological fluid mechanics Lattice Boltzmann Solver"
        HOMEPAGE_URL "https://gitlab.com/timmkrueger/biofmgroupcode"
        LANGUAGES CXX C)
include(GNUInstallDirs)

# This variable is set by project() in CMake 3.21+
string(
        COMPARE EQUAL
        "${CMAKE_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}"
        PROJECT_IS_TOP_LEVEL)
if(PROJECT_IS_TOP_LEVEL)
    # Consider the CTest module, which creates targets and options!
    # Only needed if you want to enable submissions to a CDash server.
    include(CTest)
endif()

set(GIT_SHA
        "Unknown"
        CACHE STRING "SHA this build was generated from")
string(
        SUBSTRING "${GIT_SHA}"
        0
        8
        GIT_SHORT_SHA)

get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(BUILDING_MULTI_CONFIG)
    if(NOT CMAKE_BUILD_TYPE)
        # Make sure that all supported configuration types have their
        # associated conan packages available. You can reduce this
        # list to only the configuration types you use, but only if one
        # is not forced-set on the command line for VS
        message(TRACE "Setting up multi-config build types")
        set(CMAKE_CONFIGURATION_TYPES
                Debug
                Release
                RelWithDebInfo
                MinSizeRel
                CACHE STRING "Enabled build types" FORCE)
    else()
        message(TRACE "User chose a specific build type, so we are using that")
        set(CMAKE_CONFIGURATION_TYPES
                ${CMAKE_BUILD_TYPE}
                CACHE STRING "Enabled build types" FORCE)
    endif()
endif()

include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

project_options(
        # ENABLE_CACHE
        # ENABLE_CPPCHECK
        # ENABLE_CLANG_TIDY
        ENABLE_CONAN
        ENABLE_INTERPROCEDURAL_OPTIMIZATION
        ENABLE_NATIVE_OPTIMIZATION
        # ENABLE_DOXYGEN
        # ENABLE_COVERAGE
        # ENABLE_SANITIZER_ADDRESS
        # ENABLE_SANITIZER_LEAK
        # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
        # ENABLE_PCH
        # PCH_HEADERS <vector> <string>
        # WARNINGS_AS_ERRORS
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_USER_LINKER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # CONAN_OPTIONS
)

# configure files based on CMake configuration options
# Get the latest commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%H
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
# TODO: The INTERFACE library NAMESPACE ALIAS are missing! CK
add_library(matrix_comparison::project_options INTERFACE IMPORTED)
add_library(matrix_comparison::project_warnings INTERFACE IMPORTED)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

########################################################################################################################
########################################################################################################################

# Find BLAS and lapack
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

# Find dependencies:
set(DEPENDENCIES_CONFIGURED
        armadillo
        blaze
        Catch2
        CLI11
        Eigen3
        GSL
        fmt
        pcg-cpp
        spdlog
        range-v3
        xtensor
        )

foreach (DEPENDENCY ${DEPENDENCIES_CONFIGURED})
    find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach ()


# Adding the src:
add_subdirectory(src)

# Adding the tests:
option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})
if(ENABLE_TESTING)
    enable_testing()
    message(AUTHOR_WARNING "Building Tests. Be sure to check out test/constexpr_tests.cpp for constexpr testing")
    add_subdirectory(test)
endif()

option(ENABLE_FUZZING "Enable the fuzz tests" OFF)
if(ENABLE_FUZZING)
    message(AUTHOR_WARNING "Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    add_subdirectory(fuzz_test)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
    get_all_installable_targets(all_targets)
    message("all_targets=${all_targets}")
    set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT intro)

if(CMAKE_SKIP_INSTALL_RULES)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

# Add other targets that you want installed here, be default we just package the one executable
# we know we want to ship
package_project(TARGETS matrix_test project_options project_warnings
        # FIXME: this does not work! CK
        # PRIVATE_DEPENDENCIES_CONFIGURED project_options project_warnings
        )

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
        )

include(CPack)