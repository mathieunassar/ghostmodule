#ifndef GHOST_SAVE_DATA_HPP
#define GHOST_SAVE_DATA_HPP

#include <string>
#include <vector>
#include <memory>

#include <google/protobuf/any.pb.h>

namespace ghost
{
	class SaveData
	{
	public:
		SaveData(const std::string& name);
		virtual ~SaveData() = default;

		// reads data at a specific index, or returns false if there is no such index or if the conversion fails
		template<typename DataType>
		bool get(DataType& type, size_t index) const;

		// adds data to the data set
		template<typename DataType>
		void put(const DataType& type);

		// replace the data at the given index
		template<typename DataType>
		bool replace(const DataType& type, size_t index);

		bool remove(size_t index);

		// gets the name of this data set
		const std::string& getName() const;

		// returns the size of this data set
		size_t size() const;

	protected:
		std::vector<std::shared_ptr<google::protobuf::Any>> _data;
		std::string _name;
	};

	#include "internal/SaveData.impl.hpp"
}

#endif // GHOST_SAVE_DATA_HPP
