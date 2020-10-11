#ifndef GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
#define GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include <iostream>
#include <list>

#include "../../src/persistence/DataCollectionFile.hpp"
#include "../../src/persistence/DatabaseFile.hpp"
#include "../protobuf/tests.pb.h"

std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> generateTestdata(size_t saveDataSize,
										 size_t dataPerSet, const std::shared_ptr<ghost::Database>& database = nullptr);

void compareTestData(const std::list<std::shared_ptr<ghost::internal::DataCollectionFile>>& data1,
		     const std::list<std::shared_ptr<ghost::internal::DataCollectionFile>>& data2);

#endif // GHOST_TESTS_PERSISTENCE_TEST_HELPERS_HPP
