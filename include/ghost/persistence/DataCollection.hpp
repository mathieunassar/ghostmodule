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

#ifndef GHOST_DATACOLLECTION_HPP
#define GHOST_DATACOLLECTION_HPP

#include <google/protobuf/any.pb.h>

#include <functional>
#include <limits>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace ghost
{
/**
 *	A collection of data available in the database.
 *	A collection is named and exposes methods to work on the data:
 *	- get/get_if: finds data in the collection
 *	- put: adds data to the collection
 *	- replace/replace_if: replace data in the collection
 *	- remove/remove_if: remove data from the collection
 *	Instances of this class are obtained from a ghost::Database object, which provides an
 *	object that fulfills this interface by implementing the protected methods fetch() and push().
 */
class DataCollection
{
public:
	virtual ~DataCollection() = default;
	/**
	 * @brief Reads data with the given ID, and updates the passed parameter if this method succeeds.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param type [output] data that will be gotten
	 * @param id id assigned to the data in the data set. If no data corresponds to this index, the method returns
	 *   false.
	 * @return true if the data was read and returned in "type"
	 * @return false if no data corresponds to this index,
	 *   or if the corresponding data does not match the provided type
	 */
	template <typename DataType>
	bool get(DataType& type, const std::string& id);
	/**
	 * @brief Gets all the data in this ghost::DataCollection that matches the provided filter.
	 *
	 * The filter is given the data and its ID and must return true if the data is accepted, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param filter filter to select the data.
	 * @return a map of elements of type DataType contained in this ghost::DataCollection that matches the filter.
	 *   The map's key is the data's ID in the set.
	 */
	template <typename DataType>
	std::map<std::string, DataType> get_if(
	    const std::function<bool(const DataType&, const std::string& id)>& filter);
	/**
	 * @brief Pushes data into the data set, effectively increasing the size by one.
	 *
	 * @tparam DataType Type of data that is being added. Currently this type must be a protobuf message.
	 * @param type data to put in the data set.
	 * @return the id assigned to the new data
	 */
	template <typename DataType>
	std::string put(const DataType& type);
	/**
	 * @brief If data exists with this ID, replaces it with the provided data.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param type data to add in the set
	 * @param id id assigned to the data in the data set.
	 * @return true if the data has been replaced with the given one.
	 */
	template <typename DataType>
	bool replace(const DataType& type, const std::string& id);
	/**
	 * @brief Executes the provided operation on every element contained in this ghost::DataCollection of the
	 * provided type. If the provided operation function returns true, the data passed as a parameter is updated in
	 * the ghost::DataCollection. Otherwise, no update is performed. The provided operation function is expected to
	 * modify the data passed as a parameter if an update is wanted.
	 *
	 * The provided operation is given the data and must return true if the data is accepted, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param operation	A function that may update the data passed as a parameter and return true if the
	 *  ghost::DataCollection must be updated.
	 * @return the number of elements that have been updated.
	 */
	template <typename DataType>
	size_t replace_if(const std::function<bool(DataType&, const std::string& id)>& operation);
	/**
	 * @brief Removes the data with the given ID.
	 *
	 * @param id id assigned to the data in the data set. If no data corresponds to this index, the method returns
	 *   false.
	 * @return true if data has been removed from the data set
	 * @return false if no data has been removed from the data set.
	 */
	virtual bool remove(const std::string& id) = 0;
	/**
	 * @brief Removed data from the ghost::DataCollection based on a provided condition.
	 *
	 * The provided filter is given the data and must return true if the data must be removed, and false otherwise.
	 *
	 * @tparam DataType Data type that needs to be gotten. Currently this type must be a protobuf message.
	 * @param filter filter to select the data to remove.
	 * @return the number of elements that have been removed.
	 */
	template <typename DataType>
	size_t remove_if(const std::function<bool(DataType&, const std::string& id)>& filter);
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

protected:
	/**
	 * Retrieves data from the database based on the following:
	 * - the data type must match that of the message created by the factory "messageFactory".
	 * - if the idFilter list is not empty, the ID of the returned messages must match one of the
	 *   listed ids.
	 * - if the idFilter list is empty, all messages matching the factory type must be returned.
	 * @param messageFactory function that creates a new message instance of the desired type.
	 * @param idFilter list of IDs to fetch. If the list is empty, all the matching messages must
	 *  be retrieved.
	 * @return a map containing the matching messages. The key represents the ID of the message
	 *  in the data set.
	 */
	virtual std::map<std::string, std::shared_ptr<google::protobuf::Message>> fetch(
	    const std::function<std::shared_ptr<google::protobuf::Message>()>& messageFactory,
	    std::list<std::string> idFilter = {}) = 0;
	/**
	 * Adds or replace data from the database.
	 * If the provided hint id is empty, the data is added with a new ID. Otherwise, the data is replaced.
	 * @param data the data to add to the database.
	 * @param id the hint ID to push the data.
	 * @return the id assigned to the pushed/replaced data, or an empty string if the operation failed.
	 */
	virtual std::string push(const google::protobuf::Message& data, const std::string& id = "") = 0;
};
} // namespace ghost

#include "internal/DataCollection.impl.hpp"

#endif // GHOST_DATACOLLECTION_HPP
