# Setup the search paths
xmake_import_search_paths(ATHENA_ENTITIES)
xmake_import_search_paths(UNITTEST_CPP)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_import_search_paths(ATHENA_SCRIPTING)
endif()


# List the source files
set(SRCS main.cpp
         tests/test_Animation.cpp
         tests/test_ComponentsList.cpp
         tests/test_ComponentsManager.cpp
         tests/test_Entity.cpp
         tests/test_Scene.cpp
         tests/test_ScenesManager.cpp
         tests/test_Transforms.cpp
)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(SRCS ${SRCS}
             tests/scripting.cpp
    )
endif()


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_ENTITIES UnitTests-Athena-Entities ${SRCS})

xmake_add_to_list_property(UNITTESTS_ATHENA_ENTITIES COMPILE_DEFINITIONS "ATHENA_ENTITIES_UNITTESTS_SCRIPTS_PATH=\"${ATHENA_ENTITIES_SOURCE_DIR}/unittests/Athena-Entities/scripts/\";ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH=\"${ATHENA_SCRIPTING_SOURCE_DIR}/unittests/js-test-runner/\"")

xmake_project_link(UNITTESTS_ATHENA_ENTITIES UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_ENTITIES ATHENA_ENTITIES)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_project_link(UNITTESTS_ATHENA_ENTITIES ATHENA_SCRIPTING)

    add_dependencies(UnitTests-Athena-Entities Athena-Entities-Scripting-module Run-UnitTests-Athena-Scripting)
endif()


# Run the unit tests
set(WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if (WIN32)
    set(WORKING_DIRECTORY "${WORKING_DIRECTORY}/$(OutDir)")
endif()

add_custom_target(Run-UnitTests-Athena-Entities ALL UnitTests-Athena-Entities
                  DEPENDS UnitTests-Athena-Entities
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: Athena-Entities..." VERBATIM)
