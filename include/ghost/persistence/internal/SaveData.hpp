#ifndef GHOST_INTERNAL_SAVE_DATA_HPP
#define GHOST_INTERNAL_SAVE_DATA_HPP

#include "../SaveData.hpp"

namespace ghost
{
	namespace internal
	{
		class SaveData : public ghost::SaveData
		{
		public:
			SaveData(const std::string& name, const std::vector<std::shared_ptr<google::protobuf::Any>>& data);

			const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const;
		};
	}
}

#endif // GHOST_INTERNAL_SAVE_DATA_HPP
