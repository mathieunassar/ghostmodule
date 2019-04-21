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

#ifndef GHOST_MODULE_GHOST_LOGGER_LINE_HPP
#define GHOST_MODULE_GHOST_LOGGER_LINE_HPP

#include <sstream>

namespace ghost
{
	class Logger;

	class LoggerLine
	{
	public:
		enum class Level { TRACE, DEBUG, INFO, WARN, ERROR };

		// Constructors
		LoggerLine(Logger* logger, Level level);
		~LoggerLine();

		// Methods
		template<typename T>
		LoggerLine& operator<<(const T& part) {
			_entry << part;
			return *this;
		}

	protected:
		// Attributes
		Logger* _logger;
		std::ostringstream _entry;
		Level _level;
	};
}

#endif // GHOST_MODULE_GHOST_LOGGER_LINE_HPP
