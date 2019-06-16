#ifndef GHOST_INTERNAL_SAVE_DATA_HPP
#define GHOST_INTERNAL_SAVE_DATA_HPP

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
			SaveData(const std::string& name);
			virtual ~SaveData() = default;

			void setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data);
			const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const;

		protected:
			std::vector<std::shared_ptr<google::protobuf::Any>> _data;
			std::string _name;
		};
	}
}

#endif // GHOST_INTERNAL_SAVE_DATA_HPP
