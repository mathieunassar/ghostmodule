function(generate_protoc2 source_dir output_dir source_file)
	message(STATUS "Generating grpc and protobuf: " ${source_file})

	if (GRPC_CPP_PLUGIN_PATH)
		execute_process(
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} -I ${source_dir} --grpc_out=${output_dir} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH} ${source_file}
		)
	endif()
	
	execute_process(
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} -I ${source_dir} --cpp_out=${output_dir} ${source_file}
	)
endfunction(generate_protoc2)
