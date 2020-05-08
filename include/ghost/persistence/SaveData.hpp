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

#ifndef GHOST_SAVE_DATA_HPP
#define GHOST_SAVE_DATA_HPP

#include <google/protobuf/any.pb.h>

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace ghost
{
/**
 * @brief Contains data to store in a save file.
 *
 * This object can contain Google protobuf messages, which can be managed
 * with the help of the methods "put", "get", "replace" and "remove".
 *
 * SaveData objects can be managed by the ghost::SaveManager, which can save them and
 * load them from the disk.
 *
 * Internally, the message is stored in a Google.protobuf.Any message
 * and will be serialized in a binary file along with its size, for further
 * retrieval.
 *
 * Possible extension with the ghost feature "Message" could be implemented.
 */
class SaveData
{
public:
	/**
	 * @brief Constructs a new ghost::SaveData object with the given name.
	 *
	 * @param name the name of this data set.
	 */
	static std::shared_ptr<ghost::SaveData> create(const std::string& name);

	virtual ~SaveData() = default;

	/**
	 * @brief Reads data at the given index, and updates the passed parameter if this method succeeds.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param type [output] data that will be gotten
	 * @param index position of the object in the data set. If the index is out of range, the method returns false
	 * @return true if the data was read and returned in "type"
	 * @return false if the index is out of range, of if the corresponding data does not match the provided type
	 */
	template <typename DataType>
	bool get(DataType& type, size_t index) const;

	/**
	 * @brief Gets all the data in this ghost::SaveData that matches the provided filter.
	 *
	 * The provided is given the data and must return true if the data is accepted, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param filter filter to select the data.
	 * @return a list of elements of type DataType contained in this ghost::SaveData that matches the filter.
	 */
	template <typename DataType>
	std::list<DataType> get_if(const std::function<bool(const DataType&)>& filter) const;

	/**
	 * @brief Pushes data into the data set, effectively increasing the size by one.
	 *
	 * @tparam DataType Type of data that is being added. Currently this type must be a protobuf message.
	 * @param type data to put in the data set.
	 */
	template <typename DataType>
	void put(const DataType& type);

	/**
	 * @brief If data exists at this index, replaces it with the provided data.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param type data to add in the set
	 * @param index position of the object in the data set. If the index is out of range, the method returns false
	 * @return true if the data has been replaced with the given one.
	 * @return false if the index is out of range
	 */
	template <typename DataType>
	bool replace(const DataType& type, size_t index);

	/**
	 * @brief Executes the provided operation on every element contained in this ghost::SaveData of the provided
	 * type. If the provided operation function returns true, the data passed as a parameter is updated in the
	 * ghost::SavaData. Otherwise, no update is performed. The provided operation function is expected to modified
	 * the data passed as a parameter if an update is wanted.
	 *
	 * The provided operation is given the data and must return true if the data is accepted, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param operation	A function that may update the data passed as a parameter and return true if the
	 *  ghost::SaveData must be updated.
	 * @return the number of elements that have been updated.
	 */
	template <typename DataType>
	size_t replace_if(const std::function<bool(DataType&)>& operation);

	/**
	 * @brief Removes the data at the given index.
	 *
	 * @param index position of the object in the data set. If the index is out of range, the method returns false
	 * @return true if data has been removed from the data set
	 * @return false if no data has been removed from the data set.
	 */
	virtual bool remove(size_t index) = 0;

	/**
	 * @brief Removed data from the ghost::SaveData based on a provided condition.
	 *
	 * The provided filter is given the data and must return true if the data must be removed, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param filter filter to select the data to remove.
	 * @return the number of elements that have been removed.
	 */
	template <typename DataType>
	size_t remove_if(const std::function<bool(DataType&)>& filter);

	/**
	 * @brief Gets the name of this data set
	 *
	 * @return the name of this data set
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * @brief Returns the size of this data set, i.e. the number of data that it contains.
	 *
	 * @return the size of this data set.
	 */
	virtual size_t size() const = 0;

	/**
	 * @brief Returns the data set as a vector of google.protobuf.Any objects.
	 *
	 * @return the data set as a vector of google.protobuf.Any objects.
	 */
	virtual const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const = 0;

protected:
	virtual std::vector<std::shared_ptr<google::protobuf::Any>>& getAllData() = 0;

	virtual void setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data) = 0;
};

#include "SaveData.impl.hpp"
} // namespace ghost

#endif // GHOST_SAVE_DATA_HPP
