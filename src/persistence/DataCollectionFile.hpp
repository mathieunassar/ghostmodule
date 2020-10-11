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

#ifndef GHOST_INTERNAL_DATACOLLECTIONFILE_HPP
#define GHOST_INTERNAL_DATACOLLECTIONFILE_HPP

#include <ghost/persistence/DataCollection.hpp>

namespace ghost
{
namespace internal
{
class DataCollectionFile : public ghost::DataCollection
{
public:
	DataCollectionFile(const std::string& name);

	bool remove(size_t index) override;
	const std::string& getName() const override;
	size_t size() const override;

	std::vector<std::shared_ptr<google::protobuf::Any>>& getData();
	void setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data);

protected:
	std::vector<std::shared_ptr<google::protobuf::Any>> fetch(const std::string& typeName = "") override;
	bool push(const std::shared_ptr<google::protobuf::Any>& data, int index = -1) override;

private:
	static std::string getTrueTypeName(const google::protobuf::Any& message);

	std::vector<std::shared_ptr<google::protobuf::Any>> _data;
	std::string _name;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_DATACOLLECTIONFILE_HPP
