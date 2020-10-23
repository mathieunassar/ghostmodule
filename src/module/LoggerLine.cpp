/*
 * Copyright 2020 Mathieu Nassar
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

#include <ghost/module/Logger.hpp>
#include <ghost/module/LoggerLine.hpp>

ghost::LoggerLine::LoggerLine(ghost::Logger* logger, ghost::LogLevel level) : _logger(logger), _level(level)
{
}

ghost::LoggerLine::LoggerLine(const ghost::LoggerLine&& other)
    : _logger(std::move(other._logger)), _level(std::move(other._level))
{
	// entry is left uninitialized
}

ghost::LoggerLine::~LoggerLine()
{
	switch (_level)
	{
		case ghost::LogLevel::LEVEL_TRACE:
			_logger->trace(_entry.str());
			break;
		case ghost::LogLevel::LEVEL_DEBUG:
			_logger->debug(_entry.str());
			break;
		case ghost::LogLevel::LEVEL_INFO:
			_logger->info(_entry.str());
			break;
		case ghost::LogLevel::LEVEL_WARN:
			_logger->warn(_entry.str());
			break;
		case ghost::LogLevel::LEVEL_ERROR:
			_logger->error(_entry.str());
			break;
		default:
			break;
	}
}
