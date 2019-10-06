##########################################################################################################################################
################################################################ MODULE LIBRARY ##########################################################
##########################################################################################################################################

file(GLOB header_module_lib
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/Console.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/CommandLineInterpreter.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/Command.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/CommandLine.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/PermissionEntity.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/User.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/UserGroup.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/UserManager.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/Module.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/ModuleBuilder.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/Logger.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/LoggerLine.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/GhostLogger.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/StdoutLogger.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/ModuleComponent.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/module/ModuleComponentBuilder.hpp
)

file(GLOB header_module_lib_internal_windows
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleDeviceWindows.hpp
)

file(GLOB header_module_lib_internal_unix
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleDeviceUnix.hpp
)

file(GLOB header_module_lib_internal
${GHOST_MODULE_ROOT_DIR}/src/module/Console.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/Prompt.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputController.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputControllerAccess.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleDevice.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleStream.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputEvent.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/EnterPressedInputEvent.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputModeInputEvent.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/LineRequestInputEvent.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/OutputController.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/CommandLineInterpreter.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/CommandLineParser.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/User.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/UserGroup.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/UserManager.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/Module.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/ModuleBuilder.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/GhostLogger.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/StdoutLogger.hpp
)

if (WIN32)
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_windows})
else()
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_unix})
endif()

file(GLOB header_module_lib_internal_commands
${GHOST_MODULE_ROOT_DIR}/src/module/commands/LoginCommand.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/commands/HelpCommand.hpp
${GHOST_MODULE_ROOT_DIR}/src/module/commands/ExitCommand.hpp
)

file(GLOB source_module_lib_internal_windows
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleDeviceWindows.cpp
)

file(GLOB source_module_lib_internal_unix
${GHOST_MODULE_ROOT_DIR}/src/module/ConsoleDeviceUnix.cpp
)

file(GLOB source_module_lib
${GHOST_MODULE_ROOT_DIR}/src/module/Console.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/Prompt.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputController.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/OutputController.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/EnterPressedInputEvent.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/InputModeInputEvent.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/LineRequestInputEvent.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/CommandLine.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/CommandLineParser.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/CommandLineInterpreter.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/User.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/UserGroup.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/UserManager.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/Module.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/ModuleBuilder.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/GhostLogger.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/StdoutLogger.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/LoggerLine.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/Logger.cpp
)

if (WIN32)
	set(source_module_lib ${source_module_lib} ${source_module_lib_internal_windows})
else()
	set(source_module_lib ${source_module_lib} ${source_module_lib_internal_unix})
endif()

file(GLOB source_module_lib_commands
${GHOST_MODULE_ROOT_DIR}/src/module/commands/LoginCommand.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/commands/HelpCommand.cpp
${GHOST_MODULE_ROOT_DIR}/src/module/commands/ExitCommand.cpp
)

source_group("API" FILES ${header_module_lib})
source_group("Header Files\\commands" FILES ${header_module_lib_internal_commands})
source_group("Source Files\\commands" FILES ${source_module_lib_commands})

##########################################################################################################################################

add_library(ghost_module ${header_module_lib}
	${source_module_lib}
	${header_module_lib_internal}
	${header_module_lib_internal_commands}
	${source_module_lib_commands})

target_link_libraries(ghost_module)

if (UNIX)
	target_link_libraries(ghost_module pthread)
endif()

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	file(GLOB module_tests_src
		${GHOST_MODULE_ROOT_DIR}/tests/module/CommandLineInterpreterTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/UserManagerTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/ModuleTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/InputControllerTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/OutputControllerTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/ConsoleTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/module/ConsoleDeviceMock.hpp
	)

	add_executable(module_tests ${module_tests_src})
	target_link_libraries(module_tests ghost_module ${CONAN_LIBS_GTEST})

	gtest_add_tests(TARGET module_tests)

	set_property(TARGET module_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	# minimum code to create a module
	add_executable(module_minimum ${GHOST_MODULE_ROOT_DIR}/examples/module_minimum.cpp)
	target_link_libraries(module_minimum ghost_module)

	set_property(TARGET module_minimum PROPERTY FOLDER "examples")

	# module that writes every second where the user can update the written text
	add_executable(module_with_console ${GHOST_MODULE_ROOT_DIR}/examples/module_with_console.cpp)
	target_link_libraries(module_with_console ghost_module)

	set_property(TARGET module_with_console PROPERTY FOLDER "examples")
endif()
