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

#include "Prompt.hpp"
#include <regex>

using namespace ghost::internal;

Prompt::Prompt(const std::string& format)
	: _format(format)
	, _user("")
{
	update();
}

std::string Prompt::str() const
{
	return _text;
}

void Prompt::setUser(const std::string& user)
{
	_user = user;
	update();
}

void Prompt::setFormat(const std::string& format)
{
	_format = format;
	update();
}

void Prompt::update()
{
	_text = _format;

	std::regex userRegex("\\$u");
	_text = std::regex_replace(_text, userRegex, _user);

	/*std::regex userRegex("(?<!\\$)\\$u");
	_text = std::regex_replace(_text, userRegex, _user);

	std::regex userRegex2("\\$\\$u");
	_text = std::regex_replace(_text, userRegex2, "$u");*/
}
