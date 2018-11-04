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

		// gets the name of this data set
		const std::string& getName() const;

		// returns the size of this data set
		size_t size() const;

	protected:
		std::vector<std::shared_ptr<google::protobuf::Any>> _data;
		std::string _name;
	};

	template<typename DataType>
	bool SaveData::get(DataType& type, size_t index) const
	{
		if (index >= _data.size())
			return false;

		return _data.at(index)->UnpackTo(&type);
	}

	// adds data to the data set
	template<typename DataType>
	void SaveData::put(const DataType& type)
	{
		auto any = std::make_shared<google::protobuf::Any>();
		any->PackFrom(type);
		_data.push_back(any);
	}

	template<typename DataType>
	bool SaveData::replace(const DataType& type, size_t index)
	{
		if (index >= _data.size())
			return false;

		auto any = std::make_shared<google::protobuf::Any>();
		any->PackFrom(type);
		_data[index] = any;

		return true;
	}
}

#endif // GHOST_SAVE_DATA_HPP
