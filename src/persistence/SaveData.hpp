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

#ifndef GHOST_INTERNAL_SAVE_DATA_HPP
#define GHOST_INTERNAL_SAVE_DATA_HPP

#include <ghost/persistence/SaveData.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 *	Internal implementation of ghost::SaveData.
		 */
		class SaveData : public ghost::SaveData
		{
		public:
			SaveData(const std::string& name);
			virtual ~SaveData() = default;

			bool remove(size_t index) override;
			const std::string& getName() const override;
			size_t size() const override;
			const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const override;

			std::vector<std::shared_ptr<google::protobuf::Any>>& getData() override;
			void setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data) override;

		private:
			std::vector<std::shared_ptr<google::protobuf::Any>> _data;
			std::string _name;
		};
	}
}

#endif // GHOST_INTERNAL_SAVE_DATA_HPP
