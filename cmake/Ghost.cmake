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

function(setup_ghost_repository)
	#set up the location of ghost_third_parties
	if (NOT DEFINED ENV{GHOST_THIRD_PARTIES_URL})
		message(FATAL_ERROR "Ghost third parties location unkown. Please set an environment variable named \"GHOST_THIRD_PARTIES_URL\" containing its URL")
	endif()
	
	set(GHOST_THIRD_PARTIES_ROOT_DIR $ENV{GHOST_THIRD_PARTIES_URL} CACHE STRING "Ghost Third Parties location" FORCE)

	if (EXISTS ${GHOST_THIRD_PARTIES_ROOT_DIR})
		message(STATUS "Set the URL of ghost third parties to ${GHOST_THIRD_PARTIES_ROOT_DIR}")
	else()
		message(FATAL_ERROR "The provided URL for ghost third parties does not exist, aborting.")
	endif()
endfunction(setup_ghost_repository)
