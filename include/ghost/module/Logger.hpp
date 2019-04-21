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

#ifndef GHOST_MODULE_LOGGER_HPP
#define GHOST_MODULE_LOGGER_HPP

/**
 *	Helper macros, that take a pointer to a logger as a parameter and simplifies
 *	a log call.
 */
#define GHOST_TRACE(LOGGER) LOGGER->operator()(ghost::LogLevel::LEVEL_TRACE)
#define GHOST_DEBUG(LOGGER) LOGGER->operator()(ghost::LogLevel::LEVEL_DEBUG)
#define GHOST_INFO(LOGGER) LOGGER->operator()(ghost::LogLevel::LEVEL_INFO)
#define GHOST_WARN(LOGGER) LOGGER->operator()(ghost::LogLevel::LEVEL_WARN)
#define GHOST_ERROR(LOGGER) LOGGER->operator()(ghost::LogLevel::LEVEL_ERROR)

#include <string>
#include <ghost/module/LoggerLine.hpp>

namespace ghost
{
	/**
	 * @brief Interface for a logger used in the system.
	 * 
	 * An implementation must be provided to the modules using it.
	 */
	class Logger
	{
	public:
		virtual ~Logger() = default;

		LoggerLine operator()(const LogLevel& level);
		
		/**
		 * @brief Writes a trace log line to the underlying support.
		 *
		 * @param line line to write.
		 */
		virtual void trace(const std::string& line) = 0;

		/**
		 * @brief Writes a debug log line to the underlying support.
		 *
		 * @param line line to write.
		 */
		virtual void debug(const std::string& line) = 0;

		/**
		 * @brief Writes an info log line to the underlying support.
		 *
		 * @param line line to write.
		 */
		virtual void info(const std::string& line) = 0;
		
		/**
		 * @brief Writes a warning log line to the underlying support.
		 * 
		 * @param line line to write.
		 */
		virtual void warn(const std::string& line) = 0;
		
		/**
		 * @brief Writes an error log line to the underlying support.
		 * 
		 * @param line line to write.
		 */
		virtual void error(const std::string& line) = 0;
	};
}

#endif // GHOST_MODULE_LOGGER_HPP
