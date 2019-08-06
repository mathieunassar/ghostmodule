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

template<typename MessageType>
std::shared_ptr<Writer<MessageType>> Connection::getWriter()
{
	// create a new writer with the message queue of the existing one - _writer is not nullptr because it is initialized in the constructor
	auto newWriter = std::make_shared<GenericWriter<MessageType>>(*_writer, _configuration.isOperationBlocking());
	setWriterSink(newWriter);
	return newWriter;
}

template<typename MessageType>
std::shared_ptr<Reader<MessageType>> Connection::getReader()
{
	// create a new reader with the message queue of the existing one - _reader is not nullptr because it is initialized in the constructor
	auto newReader = std::make_shared<GenericReader<MessageType>>(*_reader, _configuration.isOperationBlocking());
	setReaderSink(newReader);
	return newReader;
}
