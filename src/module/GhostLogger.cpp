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

#include "GhostLogger.hpp"
#include <iostream>

using namespace ghost::internal;

std::shared_ptr<ghost::GhostLogger> ghost::GhostLogger::create(const std::shared_ptr<ghost::Console>& console)
{
	return std::make_shared<ghost::internal::GhostLogger>(console);
}

GhostLogger::GhostLogger(const std::shared_ptr<ghost::Console>& console) : _console(console)
{
}

/* From ghost::Logger */
void GhostLogger::trace(const std::string& line)
{
	std::string toWrite = "[TRACE] " + line + "\n";
	printLine(toWrite);
}

void GhostLogger::debug(const std::string& line)
{
	std::string toWrite = "[DEBUG] " + line + "\n";
	printLine(toWrite);
}

void GhostLogger::info(const std::string& line)
{
	std::string toWrite = "[INFO ] " + line + "\n";
	printLine(toWrite);
}

void GhostLogger::warn(const std::string& line)
{
	std::string toWrite = "[WARN ] " + line + "\n";
	printLine(toWrite);
}

void GhostLogger::error(const std::string& line)
{
	std::string toWrite = "[ERROR] " + line + "\n";
	printLine(toWrite);
}

void GhostLogger::printLine(const std::string& line) const
{
	if (_console)
		_console->write(line);
	else
		std::cout << line << std::flush;
}
