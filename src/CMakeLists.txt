# List the source files
set(SRCS Animation.cpp
         AnimationsMixer.cpp
         Component.cpp
         ComponentsList.cpp
         ComponentsManager.cpp
         Entity.cpp
         Scene.cpp
         ScenesManager.cpp
         Transforms.cpp
)

# Precompiler definitions
add_definitions(-DATHENA_EXPORTS)

# List the include paths
include_directories(${ATHENA_ENTITIES_SOURCE_DIR}/include
                    ${ATHENA_CORE_SOURCE_DIR}/include
                    ${ATHENA_MATH_SOURCE_DIR}/include
                    ${ATHENA_BINARY_DIR}/include)

# Declaration of the library
if (NOT ATHENA_ENTITIES_STATIC)
    add_library(Athena-Entities SHARED ${SRCS} ${PLATFORM_SRCS})
    target_link_libraries(Athena-Entities Athena-Core Athena-Math)
else()
    add_library(Athena-Entities STATIC ${SRCS} ${PLATFORM_SRCS})
endif()
