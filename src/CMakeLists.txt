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


# List the include paths
set(INCLUDE_PATHS "${ATHENA_ENTITIES_SOURCE_DIR}/include"
                  "${XMAKE_BINARY_DIR}/include")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(ATHENA_CORE)


# Declaration of the library
if (NOT ATHENA_ENTITIES_STATIC)
    xmake_create_dynamic_library(ATHENA_ENTITIES Athena-Entities 1.0.0 1.0.0 ${SRCS})
else()
    xmake_create_static_library(ATHENA_ENTITIES Athena-Entities ${SRCS})
endif()

xmake_project_link(ATHENA_ENTITIES ATHENA_CORE)

xmake_add_to_list_property(ATHENA_ENTITIES COMPILE_DEFINITIONS "ATHENA_EXPORTS")


# Exportations
xmake_export_include_paths(ATHENA_ENTITIES ${INCLUDE_PATHS})
