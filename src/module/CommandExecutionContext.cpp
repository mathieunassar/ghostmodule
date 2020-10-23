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

#include <ghost/module/CommandExecutionContext.hpp>

using namespace ghost;

CommandExecutionContext::CommandExecutionContext(const std::shared_ptr<ghost::Session>& session) : _session(session)
{
}

void CommandExecutionContext::setConsole(const std::shared_ptr<ghost::Console>& console)
{
	_console = console;
}

const std::shared_ptr<ghost::Session>& CommandExecutionContext::getSession() const
{
	return _session;
}

std::shared_ptr<ghost::Console> CommandExecutionContext::getConsole() const
{
	return _console;
}
