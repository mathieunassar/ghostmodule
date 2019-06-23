#ifndef GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
#define GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP

#include <gtest/gtest.h>
#include <iostream>
#include <list>

#include <ghost/persistence/SaveData.hpp>
#include "../src/persistence/SaveData.hpp"
#include "protobuf/tests.pb.h"

std::list<std::shared_ptr<ghost::internal::SaveData>> generateTestdata(size_t saveDataSize, size_t dataPerSet);

void compareTestData(const std::list<std::shared_ptr<ghost::internal::SaveData>>& data1,
	const std::list<std::shared_ptr<ghost::internal::SaveData>>& data2);

#endif // GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
