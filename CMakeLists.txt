cmake_minimum_required(VERSION 2.6)
project(ATHENA_ENTITIES)

# Settings
if (NOT DEFINED ATHENA_ENTITIES_SCRIPTING)
    set(ATHENA_ENTITIES_SCRIPTING OFF CACHE BOOL "Enable scripting support (needs Athena-Scripting)")
endif()

if (NOT DEFINED ATHENA_ENTITIES_STATIC)
    set(ATHENA_ENTITIES_STATIC OFF CACHE BOOL "Build Athena-Entities as a static library")
endif()

# Set the output paths
if (NOT ATHENA_BINARY_DIR)
    set(ATHENA_BINARY_DIR "${ATHENA_ENTITIES_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/bin")
endif()

# Setup the dependencies
if (NOT DEFINED ATHENA_MATH_STATIC)
    set(ATHENA_MATH_STATIC ON)
endif()

if (NOT DEFINED ATHENA_CORE_STATIC)
    set(ATHENA_CORE_STATIC ON)
endif()

# Import XMake
if (EXISTS "${ATHENA_ENTITIES_SOURCE_DIR}/dependencies/xmake/XMake.cmake")
    include("${ATHENA_ENTITIES_SOURCE_DIR}/dependencies/xmake/XMake.cmake")
endif()

# Process the dependencies
add_subdirectory(dependencies)

# XMake settings
if (ATHENA_ENTITIES_STATIC)
    xmake_init_static(ATHENA_ENTITIES)
    xmake_add_link_paths(ATHENA_ENTITIES "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_ENTITIES Athena-Entities "${XMAKE_ATHENA_CORE_LINK_TARGETS}")
else()
    xmake_init_dynamic(ATHENA_ENTITIES)
    xmake_add_link_paths(ATHENA_ENTITIES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_ENTITIES Athena-Entities)
endif()

xmake_add_include_paths(ATHENA_ENTITIES ${ATHENA_ENTITIES_SOURCE_DIR}/include
                                        ${XMAKE_ATHENA_CORE_INCLUDE_PATHS})

# Subdirectories to process
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(unittests)
