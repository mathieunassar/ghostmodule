# MODULE LIBRARY

#####################################################################

file(GLOB header_module_lib
./include/Console.hpp
./include/ConsoleDevice.hpp
./include/ConsoleDeviceWindows.hpp
./include/InputController.hpp
./include/ConsoleStream.hpp
./include/BlockingQueue.hpp
./include/InputEvent.hpp
./include/EnterPressedInputEvent.hpp
./include/InputModeInputEvent.hpp
./include/LineRequestInputEvent.hpp)

file(GLOB source_module_lib
./src/Console.cpp
./src/ConsoleDeviceWindows.cpp
./src/InputController.cpp
./src/EnterPressedInputEvent.cpp
./src/InputModeInputEvent.cpp
./src/LineRequestInputEvent.cpp
./src/main.cpp)

#####################################################################

add_executable(GhostConsole ${header_module_lib}
	${source_module_lib})

set_property(TARGET GhostConsole PROPERTY FOLDER "Libraries")
