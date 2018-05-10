# MODULE LIBRARY

#####################################################################

file(GLOB header_module_lib
./include/Console.hpp
./include/InputController.hpp
./include/OutputController.hpp
./include/CommandLineInterpreter.hpp
./include/Command.hpp
./include/CommandLine.hpp
./include/PermissionEntity.hpp
./include/User.hpp
./include/UserGroup.hpp
./include/UserManager.hpp
)

file(GLOB header_module_lib_internal
./include/internal/Console.hpp
./include/internal/InputController.hpp
./include/internal/InputControllerAccess.hpp
./include/internal/ConsoleDevice.hpp
./include/internal/ConsoleDeviceWindows.hpp
./include/internal/ConsoleStream.hpp
./include/internal/BlockingQueue.hpp
./include/internal/InputEvent.hpp
./include/internal/EnterPressedInputEvent.hpp
./include/internal/InputModeInputEvent.hpp
./include/internal/LineRequestInputEvent.hpp
./include/internal/OutputController.hpp
./include/internal/CommandLineInterpreter.hpp
./include/internal/CommandLineParser.hpp
./include/internal/User.hpp
./include/internal/UserGroup.hpp
./include/internal/UserManager.hpp
)

file(GLOB header_module_lib_internal_commands
./include/internal/commands/LoginCommand.hpp
)

file(GLOB source_module_lib
./src/Console.cpp
./src/ConsoleDeviceWindows.cpp
./src/InputController.cpp
./src/OutputController.cpp
./src/EnterPressedInputEvent.cpp
./src/InputModeInputEvent.cpp
./src/LineRequestInputEvent.cpp
./src/CommandLine.cpp
./src/CommandLineParser.cpp
./src/CommandLineInterpreter.cpp
./src/User.cpp
./src/UserGroup.cpp
./src/UserManager.cpp
./src/main.cpp)

file(GLOB source_module_lib_commands
./src/commands/LoginCommand.cpp
)



source_group("Header Files\\internal" FILES ${header_module_lib_internal})
source_group("Header Files\\internal\\commands" FILES ${header_module_lib_internal_commands})
source_group("Source Files\\commands" FILES ${source_module_lib_commands})

#####################################################################

add_executable(GhostConsole ${header_module_lib}
	${source_module_lib}
	${header_module_lib_internal}
	${header_module_lib_internal_commands}
	${source_module_lib_commands})

set_property(TARGET GhostConsole PROPERTY FOLDER "Libraries")
