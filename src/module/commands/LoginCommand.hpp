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

#ifndef GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP
#define GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP

#include <ghost/module/Command.hpp>
#include <ghost/module/UserManager.hpp>

namespace ghost
{
namespace internal
{
class LoginCommand : public ghost::Command
{
public:
	static const std::string NAME;

	LoginCommand(std::shared_ptr<ghost::UserManager> userManager);

	bool execute(const CommandLine& commandLine, const ghost::CommandExecutionContext& context) override;

	std::string getName() const override;
	std::string getShortcut() const override;
	std::string getDescription() const override;

private:
	static const std::string _SHORTCUT;
	static const std::string _DESCRIPTION;
	static const std::string _PARAM_USERNAME;
	static const std::string _PARAM_PASSWORD;
	static const std::string _PARAM_USERNAME_SHORT;
	static const std::string _PARAM_PASSWORD_SHORT;

	std::shared_ptr<ghost::UserManager> _userManager;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP
