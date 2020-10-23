##########################################################################################################################################
######################################################### PERSISTENCE_EXTENSION LIBRARY #######################################################
##########################################################################################################################################

file(GLOB header_persistenceextension_lib
	${GHOST_MODULE_ROOT_DIR}/include/ghost/persistence_extension/PersistenceExtensionBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/include/ghost/persistence_extension/PersistenceExtension.hpp
)

file(GLOB header_persistenceextension_internal_lib
	${GHOST_MODULE_ROOT_DIR}/src/persistence_extension/PersistenceExtensionBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence_extension/PersistenceExtension.hpp
)

file(GLOB source_persistenceextension_lib
	${GHOST_MODULE_ROOT_DIR}/src/persistence_extension/PersistenceExtensionBuilder.cpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence_extension/PersistenceExtension.cpp
)

source_group("API" FILES ${header_persistenceextension_lib})

##########################################################################################################################################

add_library(ghost_persistence_extension ${header_persistenceextension_internal_lib}
	${header_persistenceextension_lib}
	${source_persistenceextension_lib}
)

target_link_libraries(ghost_persistence_extension ghost_module ghost_persistence)

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)
	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.proto)
	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)

	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

	# minimum code to create a module
	add_executable(persistence_todo_list
		${GHOST_MODULE_ROOT_DIR}/examples/persistence_todo_list.cpp
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.h
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.cc)
	target_link_libraries(persistence_todo_list ghost_module ghost_persistence_extension ghost_persistence)

	set_property(TARGET persistence_todo_list PROPERTY FOLDER "examples")
endif()
