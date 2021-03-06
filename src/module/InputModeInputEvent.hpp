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

#ifndef GHOST_INTERNAL_INPUTMODEINPUTEVENT_HPP
#define GHOST_INTERNAL_INPUTMODEINPUTEVENT_HPP

#include "InputEvent.hpp"

namespace ghost
{
namespace internal
{
/**
 *	The user pressed on enter.
 *	The handler switches the console mode to INPUT if the current mode is OUTPUT.
 */
class InputModeInputEvent : public InputEvent
{
public:
	/* Handler */
	class InputModeInputEventHandler : public InputEvent::InputEventHandler
	{
	public:
		InputModeInputEventHandler(InputControllerAccess* controller);
		bool handle(const InputEvent& event) override;

	private:
		const InputModeInputEvent& getEvent(const InputEvent& event) const;
	};

	/* Event */
	std::string getEventName() const override;
	static const std::string _NAME;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_INPUTMODEINPUTEVENT_HPP
