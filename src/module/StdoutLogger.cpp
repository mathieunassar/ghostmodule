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

#include "StdoutLogger.hpp"
#include <iostream>

using namespace ghost::internal;

std::shared_ptr<ghost::StdoutLogger> ghost::StdoutLogger::create()
{
	return std::make_shared<ghost::internal::StdoutLogger>();
}

StdoutLogger::StdoutLogger()
{

}

/* From ghost::Logger */
void StdoutLogger::trace(const std::string& line)
{
	std::string toWrite = "[TRACE] " + line + "\n";
	std::cout << toWrite << std::flush;
}

void StdoutLogger::debug(const std::string& line)
{
	std::string toWrite = "[DEBUG] " + line + "\n";
	std::cout << toWrite << std::flush;
}

void StdoutLogger::info(const std::string& line)
{
	std::string toWrite = "[INFO ] " + line + "\n";
	std::cout << toWrite << std::flush;
}

void StdoutLogger::warn(const std::string& line)
{
	std::string toWrite = "[WARN ] " + line + "\n";
	std::cout << toWrite << std::flush;
}

void StdoutLogger::error(const std::string& line)
{
	std::string toWrite = "[ERROR] " + line + "\n";
	std::cout << toWrite << std::flush;
}
