if (WIN32)
	set(PROTOC_PATH ${GHOST_CONNECTION_ROOT_DIR}/third-party/protobuf/bin/${GHOST_BUILD_NAME}/Release/protoc.exe CACHE FILEPATH "Path to the Protoc compiler")
	set(GRPC_CPP_PLUGIN_PATH ${GHOST_CONNECTION_ROOT_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release/grpc_cpp_plugin.exe CACHE FILEPATH "Path to the gRPC plugin for protoc")
endif (WIN32)

if (UNIX)
	set(PROTOC_PATH ${GHOST_CONNECTION_ROOT_DIR}/third-party/protobuf/bin/${GHOST_BUILD_NAME}/Release/protoc CACHE FILEPATH "Path to the Protoc compiler")
	set(GRPC_CPP_PLUGIN_PATH ${GHOST_CONNECTION_ROOT_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release/grpc_cpp_plugin CACHE FILEPATH "Path to the gRPC plugin for protoc")
endif (UNIX)

function(generate_protoc source_dir output_dir source_file)
	message(STATUS "Generating grpc and protobuf: " ${source_file})

	execute_process(
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} -I ${source_dir} --grpc_out=${output_dir} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH} ${source_file}
		COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} -I ${source_dir} --cpp_out=${output_dir} ${source_file}
	)
endfunction(generate_protoc)
