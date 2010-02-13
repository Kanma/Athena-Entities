# Setup the search paths
include_directories(${ATHENA_ENTITIES_SOURCE_DIR}/include
                    ${ATHENA_CORE_SOURCE_DIR}/include
                    ${ATHENA_MATH_SOURCE_DIR}/include
                    ${ATHENA_BINARY_DIR}/include
                    ${UNITTEST_CPP_SOURCE_DIR}/include)

# List the source files
file(GLOB SRCS main.cpp
               test_Animation.cpp
               test_ComponentsList.cpp
               test_ComponentsManager.cpp
               test_Entity.cpp
               test_Scene.cpp
               test_ScenesManager.cpp
               test_Transforms.cpp
)

# Create and link the executable
add_executable(UnitTests-Athena-Entities ${SRCS})

set(LIBS UnitTest++ Athena-Entities)

if (ATHENA_ENTITIES_STATIC)
    list(APPEND LIBS Athena-Core Athena-Math)
endif()

target_link_libraries(UnitTests-Athena-Entities ${LIBS})

# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Entities PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Entities ALL UnitTests-Athena-Entities
                  DEPENDS UnitTests-Athena-Entities
                  WORKING_DIRECTORY ${PATH}
                  COMMENT "Unit testing: Athena-Entities..." VERBATIM)