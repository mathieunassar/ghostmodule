include(FetchContent)

function(fetch_ghost_library name url branch)
	message(STATUS "Fetching ghost library '" ${name} "' from '" ${url} "', branch '" ${branch} "' ...")
	FetchContent_Declare(
		${name}
		GIT_REPOSITORY ${url}
		GIT_TAG        ${branch}
	)
	
	FetchContent_GetProperties(${name})
	if(NOT ${name}_POPULATED)
		FetchContent_Populate(${name})
		
		add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR})
	endif()
	
	message(STATUS "Fetching ghost library '" ${name} "' done.")
endfunction(fetch_ghost_library)
