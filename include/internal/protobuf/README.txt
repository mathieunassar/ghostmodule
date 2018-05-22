//////////////////////////////////////////////
////// HOWTO Generate the protobuf code //////
//////////////////////////////////////////////

- locate the sources: the source files are the values of the "-I" parameter. One source folder per "-I" parameter
	- "Ghost.proto" located in this directory
	- protobuf include files, used for example for "Any.proto"

- locate the grpc plugin
	- within the Ghost repository, a built version is compiled and copied under:
		ghostRepository\ExternalLibs\grpc\bin\VS2017_win32\grpc_cpp_plugin.exe
		(grpc 1.12.x branch, compiled with VS2017 arch. win32)
	- if the corresponding version is not built, build it. The following output is required:
		grpc/.build/Release: "grpc_cpp_plugin.exe", as well as library files
		grpc/include: grpc include files
		grpc/third_party/protobuf/src: protobuf include files, maybe we can get rid of the source files
		grpc/.build/third_party/protobuf/Release: "protoc", as well as library files for protobuf
		
- locate protoc

- execute protoc in the folder containing the files to build (or adapt the paths)
	- [protoc] -I [SOURCE1] -I [SOURCE2] --grpc_out=[OUTPUT_PATH] --plugin=protoc-gen-grpc=[PATH_TO_GRPC_PLUGIN] [INPUT_FILES]
	- [protoc] -I [SOURCE1] -I [SOURCE2] --cpp_out=[OUTPUT_PATH] [INPUT_FILES]

- exemple:
	protoc -I . -I C:\Users\MathieuNassar\Documents\GIT\ghostRepository\ExternalLibs\protobuf\include --grpc_out=. --plugin=protoc-gen-grpc=C:\Users\MathieuN
assar\Documents\GIT\ghostRepository\ExternalLibs\grpc\bin\VS2017_win32\grpc_cpp_plugin.exe .\Ghost.proto

	protoc -I . -I C:\Users\MathieuNassar\Documents\GIT\ghostRepository\ExternalLibs\protobuf\include --cpp_out=. .\Ghost.proto

///////////////////////////////////////////
////// TODO TODO TODO TODO TODO TODO //////
///////////////////////////////////////////

- create a CMake file to build it
- create a script to copy the necessary files from the grpc repository after building the solution
