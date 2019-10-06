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

#include "Connectivity.hpp"

using namespace ghost::internal;

const std::string ghost::Connectivity::NAME = "Connectivity";

Connectivity::Connectivity(const std::shared_ptr<ghost::ConnectionManager>& connectionManager)
    : _connectionManager(connectionManager)
{
}

std::shared_ptr<ghost::ConnectionManager> Connectivity::getConnectionManager() const
{
	return _connectionManager;
}

// From ghost::ModuleComponent
bool Connectivity::start()
{
	return true;
}

void Connectivity::stop()
{
}

std::string Connectivity::getName() const
{
	return ghost::Connectivity::NAME;
}
