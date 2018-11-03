#ifndef GHOST_SAVE_DATA_HPP
#define GHOST_SAVE_DATA_HPP

#include <string>
#include <vector>
#include <memory>

#include <google/protobuf/any.pb.h>

namespace ghost
{
	namespace internal
	{
		class SaveData
		{
		public:
			SaveData(const std::string& name, const std::vector<std::shared_ptr<google::protobuf::Any>>& data);

			// reads data at a specific index, or returns false if there is no such index or if the conversion fails
			template<typename DataType>
			bool get(DataType& type, size_t index) const;

			// adds data to the data set
			template<typename DataType>
			void put(const DataType& type);

			// gets the name of this data set
			const std::string& getName() const;

			// returns the size of this data set
			size_t size() const;

			const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const;

		private:
			std::vector<std::shared_ptr<google::protobuf::Any>> _data;
			std::string _name;
		};

		template<typename DataType>
		bool SaveData::get(DataType& type, size_t index) const
		{
			if (index >= _data.size())
				return false;
			
			// todo
			return false;
		}

		// adds data to the data set
		template<typename DataType>
		void SaveData::put(const DataType& type)
		{
			// todo
		}
	}
}

#endif // GHOST_SAVE_DATA_HPP
