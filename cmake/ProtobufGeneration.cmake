function(generate_protoc source_dir output_dir source_file)
	# generate the list of include folders
	foreach(dir ${source_dir})
		set(proto_include_dirs ${proto_include_dirs} -I ${dir})
	endforeach()

	foreach(file ${source_file})
		message(STATUS "Generating grpc and protobuf: ${file}")

		if (GRPC_CPP_PLUGIN_PATH)
			execute_process(
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} ${proto_include_dirs} --grpc_out=${output_dir} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH} ${file}
				RESULT_VARIABLE protoc_grpc_result
				ERROR_VARIABLE protoc_grpc_error_variable
			)

			if (NOT protoc_grpc_result EQUAL "0")
				message(FATAL_ERROR "Failed to generate protobuf. Error: ${protoc_grpc_error_variable}")
			endif()
		endif()
		
		execute_process(
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMAND ${PROTOC_PATH} -I ${Protobuf_INCLUDE_DIRS} -I ${source_dir} --cpp_out=${output_dir} ${file}
			RESULT_VARIABLE protoc_result
			ERROR_VARIABLE protoc_error_variable
		)

		if (NOT protoc_result EQUAL "0")
			message(FATAL_ERROR "Failed to generate protobuf. Error: ${protoc_error_variable}")
		endif()
	endforeach()
endfunction(generate_protoc)
