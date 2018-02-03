# MODULE LIBRARY

#####################################################################

file(GLOB header_module_lib/api
./include/Console.hpp
./include)

file(GLOB header_serializer_lib_internal
./include/ghostcore/serializer/internal/GHOST_StandardSerializer.hpp
./include/ghostcore/serializer/internal/TypeMap.hpp
./include/ghostcore/serializer/internal/PrimitiveSerializer.impl.hpp
./include/ghostcore/serializer/internal/PrimitiveGeneric.impl.hpp
./include/ghostcore/serializer/internal/PrimitiveSerializationPolicyGeneric.impl.hpp)

file(GLOB source_serializer_lib
./src/StandardSerializer.cpp
./src/GHOST_StandardSerializer.cpp
./src/SerializerError.cpp
./src/PrimitiveSerializer.cpp
./src/PrimitiveSerializationPolicy.cpp)

#####################################################################

add_library(GhostSerializer ${source_serializer_lib}
	${header_serializer_lib_api}
	${header_serializer_lib_internal})

target_link_libraries(GhostSerializer GhostCommons)

set_property(TARGET GhostSerializer PROPERTY FOLDER "Libraries")

# Unit tests: Primitive Serialization
file(GLOB source_test_unit_primitive
./tests/unittests/PrimitiveSerializationTests.cpp)

set(libraries_test_unit_primitive GhostSerializer)
add_executable(unittest_primitiveSerialization ${source_test_unit_primitive})
target_link_libraries(unittest_primitiveSerialization ${libraries_test_unit_primitive})
set_property(TARGET unittest_primitiveSerialization PROPERTY FOLDER "Tests/UnitTests")
add_test(GhostTest_PrimitiveSerializationTests unittest_primitiveSerialization)