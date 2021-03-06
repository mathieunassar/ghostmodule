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

#include <ghost/connection/ReadableConnection.hpp>

#include "ReaderSink.hpp"

using namespace ghost;

ReadableConnection::ReadableConnection(const ghost::ConnectionConfiguration& configuration)
    : _readerSink(std::make_shared<ghost::internal::ReaderSink>()), _blocking(configuration.isOperationBlocking())
{
}

std::shared_ptr<ghost::MessageHandler> ReadableConnection::addMessageHandler()
{
	return _readerSink->addMessageHandler();
}

std::shared_ptr<ghost::ReaderSink> ReadableConnection::getReaderSink() const
{
	return _readerSink;
}
