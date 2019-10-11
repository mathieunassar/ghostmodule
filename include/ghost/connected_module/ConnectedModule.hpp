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

#ifndef GHOST_CONNECTEDMODULE_HPP
#define GHOST_CONNECTEDMODULE_HPP

#include <ghost/connection/ConnectionManager.hpp>
#include <string>

namespace ghost
{
class ConnectedModule
{
public:
	static const std::string NAME;

	virtual ~ConnectedModule() = default;

	virtual std::shared_ptr<ghost::ConnectionManager> getConnectionManager() const = 0;
};
} // namespace ghost

#endif // GHOST_CONNECTEDMODULE_HPP
