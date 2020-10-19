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
#include <string>
#include <vector>

namespace ghost
{
namespace internal
{
class DataCollectionFile : public ghost::DataCollection
{
public:
	DataCollectionFile(const std::string& name, size_t nextId);

	bool remove(size_t index) override;
	const std::string& getName() const override;
	size_t getNextId() const;
	size_t size() const override;

	std::map<size_t, std::string>& getData();
	void setData(const std::map<size_t, std::string>& data);

protected:
	std::map<size_t, std::shared_ptr<google::protobuf::Message>> fetch(
	    const std::function<std::shared_ptr<google::protobuf::Message>()>& messageFactory,
	    std::list<size_t> idFilter = {}) override;
	bool push(const google::protobuf::Message& data, size_t index = std::numeric_limits<size_t>::max()) override;

private:
	static std::string getTrueTypeName(const google::protobuf::Any& message);

	std::map<size_t, std::string> _data;
	std::string _name;
	size_t _nextId;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_DATACOLLECTIONFILE_HPP
