cmake_minimum_required(VERSION 2.6)
project(ATHENA_ENTITIES)

# Settings
if (NOT ATHENA_ENTITIES_SCRIPTING)
    set(ATHENA_ENTITIES_SCRIPTING OFF CACHE BOOL "Enable scripting support (needs Athena-Scripting)")
endif()

# Set the output paths
if (NOT ATHENA_BINARY_DIR)
    set(ATHENA_BINARY_DIR "${ATHENA_ENTITIES_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ATHENA_ENTITIES_BINARY_DIR}/bin")
endif()

# Setup the dependencies
if (NOT ATHENA_MATH_STATIC)
    set(ATHENA_MATH_STATIC ON)
endif()

if (NOT ATHENA_CORE_STATIC)
    set(ATHENA_CORE_STATIC ON)
endif()

# Subdirectories to process
add_subdirectory(dependencies)
#add_subdirectory(include)
#add_subdirectory(src)
add_subdirectory(unittests)
