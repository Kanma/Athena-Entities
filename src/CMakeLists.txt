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
xmake_include_directories(ATHENA_ENTITIES)

# Declaration of the library
if (NOT ATHENA_ENTITIES_STATIC)
    add_library(Athena-Entities SHARED ${SRCS} ${PLATFORM_SRCS})
    xmake_target_link_libraries(Athena-Entities ATHENA_CORE)

    # Set the INSTALL_PATH so that the dynamic library can be local
    set_target_properties(Athena-Entities PROPERTIES BUILD_WITH_INSTALL_RPATH ON
                                                     INSTALL_NAME_DIR "@rpath"
    )
else()
    add_library(Athena-Entities STATIC ${SRCS} ${PLATFORM_SRCS})
endif()
