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

#ifndef GHOST_READERSINK_HPP
#define GHOST_READERSINK_HPP

#include <google/protobuf/any.pb.h>

#include <ghost/connection/MessageHandler.hpp>
#include <memory>

namespace ghost
{
/**
 *	A ghost::ReaderSink is the interface between a connection and a
 *	ghost::Reader object that can read messages coming from the connection.
 *	The sink must be used by implementations of connection types to forward
 *	messages to the user of the connection reader.
 */
class ReaderSink
{
public:
	virtual ~ReaderSink() = default;

	/**
	 *	Pushes a message into the readers connected to this sink.
	 *	@param message	the new message to pass to the readers.
	 *	@return true if the message was successfully passed.
	 */
	virtual bool put(const google::protobuf::Any& message) = 0;

	virtual std::shared_ptr<ghost::MessageHandler> addMessageHandler() = 0;

	/**
	 *	Shuts down the sink - future calls to put or get will fail.
	 *	Call this function when the connection stopped.
	 */
	virtual void drain() = 0;
};
} // namespace ghost

#endif // GHOST_READERSINK_HPP
