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

#ifndef GHOST_INTERNAL_READERSINK_HPP
#define GHOST_INTERNAL_READERSINK_HPP

#include <google/protobuf/any.pb.h>

#include <atomic>
#include <ghost/connection/ReaderSink.hpp>

#include "MessageHandler.hpp"
#include "QueuedSink.hpp"

namespace ghost
{
namespace internal
{
/**
 *	The internal implementation of the API class ghost::ReaderSink.
 *	This implementation manages google::protobuf::Any messages and
 *	shares them with with the bound connection over a blocking queue,
 *	which is a member of the QueuedSink class.
 */
class ReaderSink : public QueuedSink, public ghost::ReaderSink
{
public:
	ReaderSink();
	virtual ~ReaderSink() = default;

	// From ghost::ReaderSink
	bool put(const google::protobuf::Any& message) override;
	std::shared_ptr<ghost::MessageHandler> addMessageHandler() override;
	void drain() override;

	// gets a message from the sink
	bool get(google::protobuf::Any& message, bool blocking);

private:
	std::shared_ptr<ghost::internal::MessageHandler> _messageHandler;
	std::atomic_bool _drained;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_READERSINK_HPP
