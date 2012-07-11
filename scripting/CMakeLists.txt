# Setup the search paths
xmake_import_search_paths(ATHENA_SCRIPTING)
xmake_import_search_paths(ATHENA_ENTITIES)


# List the source files
set(SRCS module.cpp
         Component.cpp
         ComponentsList.cpp
         ComponentsManager.cpp
         Entity.cpp
         Scene.cpp
         ScenesManager.cpp
         Transforms.cpp
)

set(JS_SRCS ComponentID.js
)


# Declaration of the module
xmake_create_dynamic_library(ATHENA_ENTITIES_SCRIPTING_MODULE Athena-Entities-Scripting-module
                            "${ATHENA_ENTITIES_VERSION_MAJOR}.${ATHENA_ENTITIES_VERSION_MINOR}.${ATHENA_ENTITIES_VERSION_PATCH}"
                            "${ATHENA_ENTITIES_VERSION_MAJOR}.${ATHENA_ENTITIES_VERSION_MINOR}.${ATHENA_ENTITIES_VERSION_PATCH}"
                             ${SRCS})

xmake_project_link(ATHENA_ENTITIES_SCRIPTING_MODULE ATHENA_ENTITIES ATHENA_SCRIPTING)

create_scripting_module(ATHENA_ENTITIES_SCRIPTING_MODULE "Athena.Entities" "${JS_SRCS}")
