#ifndef GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
#define GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP

#include <catch.hpp>
#include <iostream>

#include <ghost/persistence/internal/SaveFile.hpp>
#include "protobuf/tests.pb.h"

std::list<std::shared_ptr<ghost::SaveData>> generateTestdata(size_t saveDataSize, size_t dataPerSet);

void compareTestData(const std::list<std::shared_ptr<ghost::SaveData>>& data1,
	const std::list<std::shared_ptr<ghost::SaveData>>& data2);

#endif // GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
