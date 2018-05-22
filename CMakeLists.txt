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

file(GLOB protobuf_connection2_lib
./protobuf/Ghost.pb.h
./protobuf/Ghost.grpc.pb.h
./protobuf/Ghost.pb.cc
./protobuf/Ghost.grpc.pb.cc
)

file(GLOB source_connection2_lib
./src/main.cpp)

source_group("Header Files\\internal" FILES ${header_connection2_lib_internal})
source_group("Protobuf" FILES ${protobuf_connection2_lib})

#####################################################################

add_executable(GhostConnection2 ${header_connection2_lib}
	${source_connection2_lib}
	${header_connection2_lib_internal}
	${protobuf_connection2_lib}
	)

target_link_libraries(GhostConnection2 libprotobuf grpc grpc++)

set_property(TARGET GhostConnection2 PROPERTY FOLDER "Libraries")
