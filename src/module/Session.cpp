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

#include "Session.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::Session> ghost::Session::create()
{
	return std::make_shared<ghost::internal::Session>("NOOOOOOOOOOOOOO");
}

std::shared_ptr<ghost::Session> ghost::Session::createLocal()
{
	return std::make_shared<ghost::internal::Session>("");
}

Session::Session(const std::string& uuid) : _uuid(uuid)
{
}

const std::string& Session::getUUID() const
{
	return _uuid;
}
