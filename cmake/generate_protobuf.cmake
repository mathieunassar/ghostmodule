function(generate_protoc source_dir output_dir source_file)
	if (NOT EXISTS ${output_dir})
		file(MAKE_DIRECTORY ${output_dir})
	endif()

	# generate the list of include folders
	foreach(dir ${source_dir})
		set(proto_include_dirs ${proto_include_dirs} -I ${dir})
	endforeach()

	set(PROTOC_INCLUDES ${CONAN_INCLUDE_DIRS_PROTOC_INSTALLER})

	if (WIN32)
		set(PROTOC_PATH "${CONAN_BIN_DIRS_PROTOC_INSTALLER}/protoc.exe")
		set(GRPC_PLUGIN "${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin.exe")
	elseif (UNIX)
		set(PROTOC_PATH "${CONAN_BIN_DIRS_PROTOC_INSTALLER}/protoc")
		set(GRPC_PLUGIN "${CONAN_BIN_DIRS_GRPC}/grpc_cpp_plugin")
	endif()

	foreach(file ${source_file})
		message(STATUS "Generating grpc and protobuf: ${file}")

		if (EXISTS ${GRPC_PLUGIN})
			execute_process(
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMAND ${PROTOC_PATH} -I ${PROTOC_INCLUDES} ${proto_include_dirs} --grpc_out=${output_dir} --plugin=protoc-gen-grpc=${GRPC_PLUGIN} ${file}
				RESULT_VARIABLE protoc_grpc_result
				ERROR_VARIABLE protoc_grpc_error_variable
			)

			if (NOT protoc_grpc_result EQUAL "0")
				message(FATAL_ERROR "Failed to generate protobuf. Error: ${protoc_grpc_error_variable}")
			endif()
		endif()
		
		execute_process(
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMAND ${PROTOC_PATH} -I ${PROTOC_INCLUDES} ${proto_include_dirs} --cpp_out=${output_dir} ${file}
			RESULT_VARIABLE protoc_result
			ERROR_VARIABLE protoc_error_variable
		)

		if (NOT protoc_result EQUAL "0")
			message(FATAL_ERROR "Failed to generate protobuf. Error: ${protoc_error_variable}")
		endif()
	endforeach()
endfunction(generate_protoc)

function(find_proto_files directory)
	file(GLOB_RECURSE find_result "${directory}*.proto")
	set(find_proto_files_result ${find_proto_files_result} ${find_result} PARENT_SCOPE)
endfunction(find_proto_files)
