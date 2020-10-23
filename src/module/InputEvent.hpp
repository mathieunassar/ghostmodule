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

#ifndef GHOST_INTERNAL_INPUTEVENT_HPP
#define GHOST_INTERNAL_INPUTEVENT_HPP

#include <string>

namespace ghost
{
namespace internal
{
class InputControllerAccess;

class InputEvent
{
public:
	virtual ~InputEvent() = default;

	class InputEventHandler
	{
	public:
		InputEventHandler(InputControllerAccess* controller) : _controller(controller)
		{
		}
		virtual ~InputEventHandler() = default;

		virtual bool handle(const InputEvent& event) = 0;

	protected:
		InputControllerAccess& getController() const
		{
			return *_controller;
		}

	private:
		InputControllerAccess* _controller;
	};

	virtual std::string getEventName() const = 0;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_INPUTEVENT_HPP
