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

#include "../include/ghost/connection/internal/Connection.hpp"
#include "../include/ghost/connection/Connection.hpp"

ghost::Connection::Connection(const ConnectionConfiguration& configuration)
	: internal::Connection(configuration)
{

}


using namespace ghost::internal;

/**
 *	This constructor sets up a default version of the reader and writer sinks,
 *	so that the connection can access a message queue.
 *	New sinks should overtake the message queue by using the dedicated *Sink class construtctor.
 */
Connection::Connection(const ghost::ConnectionConfiguration& configuration)
	: _configuration(configuration)
	, _writer(std::make_shared<GenericWriter<ghost::Message>>(configuration.isOperationBlocking()))
	, _reader(std::make_shared<GenericReader<ghost::Message>>(configuration.isOperationBlocking()))
{

}

/// a new sink is available - default implementation: replace the member variable
void Connection::setWriterSink(std::shared_ptr<WriterSink> sink)
{
	_writer = sink;
}

/// a new sink is available - default implementation: replace the member variable
void Connection::setReaderSink(std::shared_ptr<ReaderSink> sink)
{
	_reader = sink;
}

/// returns a shared_ptr to the currently configured writer sink
std::shared_ptr<WriterSink> Connection::getWriterSink() // returns a new instance of the shared_ptr, so that replacing the member is safe
{
	return _writer;
}

/// returns a shared_ptr to the currently configured reader sink
std::shared_ptr<ReaderSink> Connection::getReaderSink() // returns a new instance of the shared_ptr, so that replacing the member is safe
{
	return _reader;
}

void Connection::forwardReaderSink(Connection& to)
{
	to._reader = _reader;
}

/// copies the current writer sink to the other connection
void Connection::forwardWriterSink(Connection& to)
{
	to._writer = _writer;
}
