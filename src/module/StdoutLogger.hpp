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

#ifndef GHOST_MODULE_INTERNAL_STDOUT_LOGGER_HPP
#define GHOST_MODULE_INTERNAL_STDOUT_LOGGER_HPP

#include <ghost/module/Console.hpp>
#include <ghost/module/StdoutLogger.hpp>

namespace ghost
{
namespace internal
{
class StdoutLogger : public ghost::StdoutLogger
{
public:
	StdoutLogger();

	/* From ghost::Logger */
	void trace(const std::string& line) override;
	void debug(const std::string& line) override;
	void info(const std::string& line) override;
	void warn(const std::string& line) override;
	void error(const std::string& line) override;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_MODULE_INTERNAL_STDOUT_LOGGER_HPP
