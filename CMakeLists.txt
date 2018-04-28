# MODULE LIBRARY

#####################################################################

file(GLOB header_module_lib
./include/Console.hpp
./include/LocalConsole.hpp
./include/LocalConsoleWindows.hpp
./include/ConsoleStream.hpp)

file(GLOB source_module_lib
./src/Console.cpp
./src/LocalConsoleWindows.cpp
./src/main.cpp)

#####################################################################

add_executable(GhostConsole ${header_module_lib}
	${source_module_lib})

set_property(TARGET GhostConsole PROPERTY FOLDER "Libraries")
