# Setup the search paths
xmake_import_search_paths(ATHENA_ENTITIES)
xmake_import_search_paths(UNITTEST_CPP)


# List the source files
set(SRCS main.cpp
         test_Animation.cpp
         test_ComponentsList.cpp
         test_ComponentsManager.cpp
         test_Entity.cpp
         test_Scene.cpp
         test_ScenesManager.cpp
         test_Transforms.cpp
)


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_ENTITIES UnitTests-Athena-Entities ${SRCS})

xmake_project_link(UNITTESTS_ATHENA_ENTITIES UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_ENTITIES ATHENA_ENTITIES)


# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Entities PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Entities ALL UnitTests-Athena-Entities
                  DEPENDS UnitTests-Athena-Entities
                  WORKING_DIRECTORY ${PATH}
                  COMMENT "Unit testing: Athena-Entities..." VERBATIM)
