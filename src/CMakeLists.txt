if ((NOT DEFINED BUILD_MODULE) OR (${BUILD_MODULE}))
	add_subdirectory(module)
endif()

if ((NOT DEFINED BUILD_PERSISTENCE) OR (${BUILD_PERSISTENCE}))
	add_subdirectory(persistence)
endif()
