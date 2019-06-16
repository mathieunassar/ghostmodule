##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

##########################################################################################################################################

file(GLOB header_persistence_lib
	${GHOST_PERSISTENCE_ROOT_DIR}/include/ghost/persistence/SaveData.hpp
	${GHOST_PERSISTENCE_ROOT_DIR}/include/ghost/persistence/SaveData.impl.hpp
	${GHOST_PERSISTENCE_ROOT_DIR}/include/ghost/persistence/SaveManager.hpp
)

file(GLOB header_persistence_internal_lib
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveFile.hpp
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveData.hpp
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveManager.hpp
)

file(GLOB source_persistence_lib
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveFile.cpp
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveData.cpp
	${GHOST_PERSISTENCE_ROOT_DIR}/src/persistence/SaveManager.cpp
)

source_group("API" FILES ${header_persistence_lib})

##########################################################################################################################################

add_library(ghost_persistence ${header_persistence_internal_lib}
	${header_persistence_lib}
	${source_persistence_lib}
)

#if (WIN32)
#	target_link_libraries(ghost_persistence ${Protobuf_LIBRARIES})
#else ()
#	target_link_libraries(ghost_persistence ${Protobuf_LIBRARIES} pthread)
#endif()

##########################################################################################################################################

set(PROTOC_SOURCE_CUSTOM ${GHOST_PERSISTENCE_ROOT_DIR}/tests/protobuf/)
set(PROTOC_SOURCE_FILE ${GHOST_PERSISTENCE_ROOT_DIR}/tests/protobuf/tests.proto)
set(PROTOC_OUTPUT ${GHOST_PERSISTENCE_ROOT_DIR}/tests/protobuf/)

#generate_protoc2(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	file(GLOB source_persistence_tests
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/SaveFileTests.cpp
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/SaveDataTests.cpp
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/SaveManagerTests.cpp
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/PersistenceTestHelpers.cpp
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/PersistenceTestHelpers.hpp
	)

	file(GLOB protobuf_persistence_tests
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/protobuf/tests.pb.h
		${GHOST_PERSISTENCE_ROOT_DIR}/tests/protobuf/tests.pb.cc
	)

	add_executable(persistence_tests ${source_persistence_tests} ${protobuf_persistence_tests})
	target_link_libraries(persistence_tests ghost_persistence)

	gtest_add_tests(TARGET persistence_tests)

	set_property(TARGET persistence_tests PROPERTY FOLDER "tests")
endif()
