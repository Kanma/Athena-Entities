# Setup the search paths
xmake_include_directories(ATHENA_ENTITIES)
xmake_include_directories(UNITTEST_CPP)

# List the source files
file(GLOB SRCS main.cpp
               test_Animation.cpp
               test_ComponentsList.cpp
               test_ComponentsManager.cpp
               # test_Entity.cpp
               test_Scene.cpp
               test_ScenesManager.cpp
               test_Transforms.cpp
)

# Create and link the executable
add_executable(UnitTests-Athena-Entities ${SRCS})

xmake_target_link_libraries(UnitTests-Athena-Entities UNITTEST_CPP)
xmake_target_link_libraries(UnitTests-Athena-Entities ATHENA_ENTITIES)

# Set the RPATH
if (APPLE)
    set_target_properties(UnitTests-Athena-Entities PROPERTIES LINK_FLAGS "-Wl,-rpath,@loader_path/.")
elseif (UNIX)
    set_target_properties(UnitTests-Athena-Entities PROPERTIES INSTALL_RPATH ".")
endif()

# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Entities PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Entities ALL UnitTests-Athena-Entities
                  DEPENDS UnitTests-Athena-Entities
                  WORKING_DIRECTORY ${PATH}
                  COMMENT "Unit testing: Athena-Entities..." VERBATIM)
