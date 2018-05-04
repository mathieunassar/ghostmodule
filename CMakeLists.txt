# MODULE LIBRARY

#####################################################################

file(GLOB header_module_lib
./include/Console.hpp
./include/InputController.hpp
./include/OutputController.hpp
)

file(GLOB header_module_lib_internal
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
)

file(GLOB source_module_lib
./src/Console.cpp
./src/ConsoleDeviceWindows.cpp
./src/InputController.cpp
./src/OutputController.cpp
./src/EnterPressedInputEvent.cpp
./src/InputModeInputEvent.cpp
./src/LineRequestInputEvent.cpp
./src/main.cpp)

source_group("Header Files\\internal" FILES ${header_module_lib_internal})

#####################################################################

add_executable(GhostConsole ${header_module_lib}
	${source_module_lib}
	${header_module_lib_internal})

set_property(TARGET GhostConsole PROPERTY FOLDER "Libraries")
