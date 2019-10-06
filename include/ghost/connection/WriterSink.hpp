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

#ifndef GHOST_WRITERSINK_HPP
#define GHOST_WRITERSINK_HPP

#include <google/protobuf/any.pb.h>

#include <chrono>

namespace ghost
{
/**
 *	A ghost::WriterSink is the interface between a connection and a
 *	ghost::Writer object that can write messages to the connection.
 *	The sink must be used by implementations of connection types.
 */
class WriterSink
{
public:
	virtual ~WriterSink() = default;

	/**
	 *	Gets a message from the writing object. The call is blocking
	 *	for the duration specified in the parameter "timeout". If this duration
	 *	is set to zero, the call is not blocking.
	 *	This call does not remove the message from the writer. To do this, call
	 *	ghost::WriterSink::pop().
	 *	@param messsage	Message gotten by the writer.
	 *	@param timeout	this call will be blocking for this duration.
	 *	@return true if a message was gotten from the writer.
	 */
	virtual bool get(google::protobuf::Any& message, std::chrono::milliseconds timeout) = 0;

	/**
	 *	Remove the last message from the queue. This method must be called after the message
	 *	is effectively sent.
	 */
	virtual void pop() = 0;

	/**
	 *	Shuts down the sink - future calls to put or get will fail.
	 *	Call this function when the connection stopped.
	 */
	virtual void drain() = 0;
};
} // namespace ghost

#endif // GHOST_WRITERSINK_HPP
