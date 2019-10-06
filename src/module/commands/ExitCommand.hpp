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

#ifndef GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP
#define GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP

#include <ghost/module/Command.hpp>

namespace ghost
{
namespace internal
{
class Module;

class ExitCommand : public ghost::Command
{
public:
	static const std::string NAME;

	ExitCommand(Module* interpreter);

	bool execute(const CommandLine& commandLine) override;

	std::string getName() const override;
	std::string getShortcut() const override;
	std::string getDescription() const override;

private:
	static const std::string _SHORTCUT;
	static const std::string _DESCRIPTION;

	Module* _module;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP
