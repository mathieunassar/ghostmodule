/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PersistenceTestHelpers.hpp"

using namespace ghost::internal;

std::list<std::shared_ptr<ghost::internal::SaveData>> generateTestdata(size_t saveDataSize, size_t dataPerSet)
{
	std::list<std::shared_ptr<ghost::internal::SaveData>> testData;
	for (int j = 0; j < saveDataSize; j++)
	{
		std::vector<std::shared_ptr<google::protobuf::Any>> data;
		for (int i = 0; i < dataPerSet; i++)
		{
			std::string field1Value = "field1" + std::to_string(i) + std::to_string(j);
			auto msg = ghost::internal::protobuf::TestMessage1::default_instance();
			msg.set_field1(field1Value);
			auto any = std::make_shared<google::protobuf::Any>();
			any->PackFrom(msg);
			data.push_back(any);
		}
		std::string saveDataName = "super" + std::to_string(j);
		auto savedata = ghost::SaveData::create(saveDataName);
		auto saveDataInt = std::dynamic_pointer_cast<ghost::internal::SaveData>(savedata);
		saveDataInt->setData(data);
		
		testData.push_back(saveDataInt);
	}
	return testData;
}

void compareTestData(const std::list<std::shared_ptr<ghost::internal::SaveData>>& data1,
	const std::list<std::shared_ptr<ghost::internal::SaveData>>& data2)
{
	auto it = data1.begin();
	auto it2 = data2.begin();
	while (it != data1.end())
	{
		ASSERT_TRUE(it2 != data2.end());
		ASSERT_TRUE((*it)->getName() == (*it2)->getName());
		
		auto testData = std::dynamic_pointer_cast<ghost::internal::SaveData>(*it);
		auto testData2 = std::dynamic_pointer_cast<ghost::internal::SaveData>(*it2);
		ASSERT_TRUE(testData && testData2);

		auto d1 = testData->getData();
		auto d2 = testData2->getData();

		ASSERT_TRUE(d1.size() == d2.size());
		for (size_t i = 0; i < d1.size(); i++)
		{
			const auto& elem = d1[i];
			const auto& elem2 = d2[i];

			auto msg1 = ghost::internal::protobuf::TestMessage1::default_instance();
			auto msg2 = ghost::internal::protobuf::TestMessage1::default_instance();
			bool unpack1 = elem->UnpackTo(&msg1);
			bool unpack2 = elem2->UnpackTo(&msg2);
			ASSERT_TRUE(unpack1);
			ASSERT_TRUE(unpack2);
			ASSERT_TRUE(msg1.field1() == msg2.field1());
		}

		it++;
		it2++;
	}
	
	ASSERT_TRUE(it2 == data2.end());
}
