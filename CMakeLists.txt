# CONNECTION LIBRARY

#####################################################################

file(GLOB header_connection2_lib
./include/Connection.hpp
./include/Server.hpp
./include/Client.hpp
./include/Publisher.hpp
./include/Subscriber.hpp
)

file(GLOB header_connection2_lib_internal
./include/internal/Connection.hpp
./include/internal/Server.hpp
./include/internal/Client.hpp
./include/internal/Publisher.hpp
./include/internal/Subscriber.hpp
)

file(GLOB source_connection2_lib
./src/main.cpp)



source_group("Header Files\\internal" FILES ${header_connection2_lib_internal})

#####################################################################

add_executable(GhostConnection2 ${header_connection2_lib}
	${source_connection2_lib}
	${header_connection2_lib_internal}
	)

set_property(TARGET GhostConnection2 PROPERTY FOLDER "Libraries")
