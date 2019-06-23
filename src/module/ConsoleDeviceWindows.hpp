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

#ifndef GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <atomic>

#include "ConsoleDevice.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		*	Implementation of a {@ref ConsoleDevice} for Windows.
		*/
		class ConsoleDeviceWindows : public ConsoleDevice
		{
		public:
			ConsoleDeviceWindows();

			bool start() override;

			bool setConsoleMode(ConsoleMode mode) override;

			bool awaitInputMode() override;

			void stop() override;

		private:
			HANDLE _hStdin;
			DWORD _originalConsoleMode;
			std::atomic<bool> _enable;
		};
	}
}

#endif // GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP
