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

#ifndef GHOST_INTERNAL_CONSOLEDEVICE_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_HPP

#include <chrono>

namespace ghost
{
	namespace internal
	{
		/**
		*	Abstract class describing the console window.
		*	Since its methods operate on a lower level, implementations
		*	of this class are operating system specific.
		*/
		class ConsoleDevice
		{
		public:
			/**
			*	Describes whether the user can see their input on the console
			*	or not.
			*/
			enum ConsoleMode
			{
				INPUT, /// The user can see their input in the console
				OUTPUT /// The user cannot see their input in the console
			};

			/**
			 *	Performs the necessary initialization actions.
			 *	@return true if the initialization is successful, false otherwise.
			 */
			virtual bool start() = 0;

			/**
			 *	Performs necessary operations on the local device to dispose it properly.
			 */
			virtual void stop() = 0;

			/**
			*	Switches the mode of the console window.
			*	Depending on the "mode" parameter, this method enables or diables
			*	the visualisation of text in the console window (@see ConsoleMode)
			*	@param mode		new mode to set
			*	@return true on success, false otherwise
			*/
			virtual bool setConsoleMode(ConsoleMode mode) = 0;

			/**
			 *	Waits that the user presses enter to switch to input mode.
			 *	@return true if the user pressed enter.
			 */
			virtual bool awaitInputMode() = 0;

			/**
			 *	Reads a line from the console device.
			 *	@param output	Output parameter containing the result if this method returns true
			 *	@return true if a line was read, false otherwise.
			 */
			virtual bool read(std::string& output) = 0;

			/**
			 *	Writes the given line to the console device.
			 *	@param text		text to write to the console.
			 *	@return true if the text was successfully written, false otherwise.
			 */
			virtual bool write(const std::string& text) = 0;
		};
	}
}

#endif //GHOST_INTERNAL_CONSOLEDEVICE_HPP
