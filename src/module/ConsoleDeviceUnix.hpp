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

#ifndef GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP

#include <termios.h>
#include <unistd.h>

#include <atomic>
#include <functional>

#include "ConsoleDevice.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Implementation of a {@ref ConsoleDevice} for Windows.
 */
class ConsoleDeviceUnix : public ConsoleDevice
{
public:
	ConsoleDeviceUnix();

	bool start() override;
	void stop() override;

	bool setConsoleMode(ConsoleMode mode) override;
	bool awaitInputMode(
	    const std::chrono::steady_clock::duration& timeout = std::chrono::milliseconds(-1)) override;
	bool read(std::string& output, bool secret) override;
	bool write(const std::string& text) override;

private:
	enum class DeviceMode
	{
		IDLE,
		AWAIT_INPUT,
		READ
	};

	bool awaitInput(const std::function<bool()>& untilPredicate,
			const std::chrono::steady_clock::duration& timeout);

	termios _referenceState;
	std::atomic<bool> _enable;
	std::atomic<DeviceMode> _mode;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP
