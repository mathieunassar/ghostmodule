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

#ifndef GHOST_TESTS_CONSOLEDEVICEMOCK_HPP
#define GHOST_TESTS_CONSOLEDEVICEMOCK_HPP

#include <gmock/gmock.h>

#include "../src/module/ConsoleDevice.hpp"
#include "../src/module/InputEvent.hpp"

class ConsoleDeviceMock : public ghost::internal::ConsoleDevice
{
public:
	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, void());
	MOCK_METHOD1(setConsoleMode, bool(ConsoleMode mode));
	MOCK_METHOD1(awaitInputMode,
		     bool(const std::chrono::steady_clock::duration& timeout));
	MOCK_METHOD2(read, bool(std::string& output, bool secret));
	MOCK_METHOD1(write, bool(const std::string& text));
};

#endif // GHOST_TESTS_CONSOLEDEVICEMOCK_HPP
