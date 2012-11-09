# File generation
configure_file(${ATHENA_ENTITIES_SOURCE_DIR}/src/module.cpp.in
               ${XMAKE_BINARY_DIR}/generated/Athena-Entities/module.cpp)


# List the headers files
set(HEADERS ${XMAKE_BINARY_DIR}/include/Athena-Entities/Config.h
            ../include/Athena-Entities/Animation.h
            ../include/Athena-Entities/AnimationsMixer.h
            ../include/Athena-Entities/Component.h
            ../include/Athena-Entities/ComponentAnimation.h
            ../include/Athena-Entities/ComponentsList.h
            ../include/Athena-Entities/ComponentsManager.h
            ../include/Athena-Entities/Entity.h
            ../include/Athena-Entities/Prerequisites.h
            ../include/Athena-Entities/Scene.h
            ../include/Athena-Entities/ScenesManager.h
            ../include/Athena-Entities/Signals.h
            ../include/Athena-Entities/Transforms.h
            ../include/Athena-Entities/tComponentID.h
)


# List the source files
set(SRCS ${XMAKE_BINARY_DIR}/generated/Athena-Entities/module.cpp
         Animation.cpp
         AnimationsMixer.cpp
         Component.cpp
         ComponentsList.cpp
         ComponentsManager.cpp
         Entity.cpp
         Scene.cpp
         ScenesManager.cpp
         Transforms.cpp
)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(HEADERS ${HEADERS} ../include/Athena-Entities/Scripting.h)
    set(SRCS ${SRCS} Scripting.cpp)
endif()


# List the include paths
set(INCLUDE_PATHS "${ATHENA_ENTITIES_SOURCE_DIR}/include"
                  "${XMAKE_BINARY_DIR}/include")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(ATHENA_CORE)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_import_search_paths(ATHENA_SCRIPTING)
endif()


# Declaration of the library
xmake_create_dynamic_library(ATHENA_ENTITIES Athena-Entities
                             "${ATHENA_ENTITIES_VERSION_MAJOR}.${ATHENA_ENTITIES_VERSION_MINOR}.${ATHENA_ENTITIES_VERSION_PATCH}"
                             "${ATHENA_ENTITIES_VERSION_MAJOR}.${ATHENA_ENTITIES_VERSION_MINOR}.${ATHENA_ENTITIES_VERSION_PATCH}"
                             ${HEADERS} ${SRCS})

xmake_add_to_list_property(ATHENA_ENTITIES COMPILE_DEFINITIONS "ATHENA_ENTITIES_EXPORTS")

xmake_project_link(ATHENA_ENTITIES ATHENA_CORE)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_project_link(ATHENA_ENTITIES ATHENA_SCRIPTING)
endif()


# Exportations
xmake_export_include_paths(ATHENA_ENTITIES ${INCLUDE_PATHS})
