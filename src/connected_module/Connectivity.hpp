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

#ifndef GHOST_INTERNAL_CONNECTIVITY_HPP
#define GHOST_INTERNAL_CONNECTIVITY_HPP

#include <ghost/connected_module/Connectivity.hpp>
#include <ghost/module/ModuleComponent.hpp>

namespace ghost
{
namespace internal
{
class Connectivity : public ghost::Connectivity, public ghost::ModuleComponent
{
public:
	Connectivity(const std::shared_ptr<ghost::ConnectionManager>& connectionManager);

	// From ghost::Connectivity
	std::shared_ptr<ghost::ConnectionManager> getConnectionManager() const override;

	// From ghost::ModuleComponent
	bool start() override;
	void stop() override;
	std::string getName() const override;

private:
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONNECTIVITY_HPP
