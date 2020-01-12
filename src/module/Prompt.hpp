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

#ifndef GHOST_INTERNAL_PROMPT_HPP
#define GHOST_INTERNAL_PROMPT_HPP

#include <string>

namespace ghost
{
namespace internal
{
class Prompt
{
public:
	Prompt(const std::string& format);

	std::string str() const;

	void setUser(const std::string& user);
	void setFormat(const std::string& format);

private:
	void update();

	std::string _text;
	std::string _format;
	std::string _user;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_PROMPT_HPP
