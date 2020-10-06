##########################################################################################################################################
################################################################ SYSTEMTEST ##############################################################
##########################################################################################################################################

file(GLOB header_systemtest
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/SystemtestExecutorModule.hpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/SystemtestCommand.hpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/StopSystemtestCommand.hpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/Systemtest.hpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/ConnectionStressTest.hpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/ConnectionMonkeyTest.hpp
)

file(GLOB source_systemtest
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/systemtest_executor.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/SystemtestExecutorModule.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/SystemtestCommand.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/StopSystemtestCommand.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/Systemtest.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/ConnectionStressTest.cpp
	${GHOST_MODULE_ROOT_DIR}/tests/systemtest/ConnectionMonkeyTest.cpp
)

##########################################################################################################################################

if ((DEFINED BUILD_SYSTEMTESTS) AND (${BUILD_SYSTEMTESTS}))
	add_executable(ghost_systemtest ${header_systemtest} ${source_systemtest})
	target_link_libraries(ghost_systemtest ghost_module ghost_persistence ghost_connection ghost_connection_grpc CONAN_PKG::gtest)
	set_property(TARGET ghost_systemtest PROPERTY FOLDER "tests")
endif()
