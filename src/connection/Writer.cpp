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

#include "Writer.hpp"

using namespace ghost::internal;

// specialization of ghost::Writer<>::create to introduce the internal implementation

template<>
std::shared_ptr<ghost::Writer<google::protobuf::Any>> ghost::Writer<google::protobuf::Any>::create(const std::shared_ptr<ghost::WriterSink>& sink, bool blocking)
{
	return std::make_shared<ghost::internal::Writer>(sink, blocking);
}

// Implementation of the internal part (ghost::internal::Writer)

Writer::Writer(const std::shared_ptr<ghost::WriterSink>& sink, bool blocking)
	: _blocking(blocking)
{
	auto internalSink = std::dynamic_pointer_cast<ghost::internal::WriterSink>(sink);
	if (!internalSink)
		throw std::invalid_argument("The provided connection does not have a valid writer sink.");

	_writerSink = internalSink;
}

bool Writer::write(const google::protobuf::Any& message)
{
	return _writerSink->push(message, _blocking);
}
