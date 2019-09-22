/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless ASSERT_TRUEd by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <google/protobuf/wrappers.pb.h>

#include "ConnectionTestUtils.hpp"
#include <ghost/connection/ReadableConnection.hpp>
#include "../src/connection/ReaderSink.hpp"
#include "../src/connection/WriterSink.hpp"

 /**
  *	This test class groups the following test categories:
  *	- MessageHandler
  *	- Reader, ReaderSink and ReadableConnection
  *	- Writer, WriterSink and WritableConnection
  */

class ReaderWriterTests : public testing::Test, public GhostMessageTester
{
protected:
	void SetUp() override
	{
		_doubleValue.set_value(TEST_DOUBLE_VALUE);

		setupReader();
		setupWriter();
		setupGhostMessages();
	}

	void TearDown() override
	{

	}

	void setupReader()
	{
		auto readableConnection = std::make_shared<ReadableConnectionWithSink>(_config);
		_readable = readableConnection;
		_readerSink = std::dynamic_pointer_cast<ghost::internal::ReaderSink>(readableConnection->getSink());
		ASSERT_TRUE(_readerSink);
	}

	void setupWriter()
	{
		auto writableConnection = std::make_shared<WritableConnectionWithSink>(_config);
		_writable = writableConnection;
		_writerSink = std::dynamic_pointer_cast<ghost::internal::WriterSink>(writableConnection->getSink());
		ASSERT_TRUE(_writerSink);
	}

	void getFromWriterSink(bool expected = true)
	{
		google::protobuf::Any any;
		bool getResult = _writerSink->get(any, std::chrono::seconds(1));
		ASSERT_TRUE(getResult == expected);
		_writerSink->pop();
	}

	void putToReadersink(const google::protobuf::Message& message, bool expected = true)
	{
		google::protobuf::Any any;
		any.PackFrom(message);

		bool putResult = _readerSink->put(any);
		ASSERT_TRUE(putResult == expected);
	}

	void putToReadersink(const ghost::Message& message, bool expected = true)
	{
		google::protobuf::Any any;
		bool creationSuccess = ghost::internal::GenericMessageConverter::create(any, message);
		ASSERT_TRUE(creationSuccess);

		bool putResult = _readerSink->put(any);
		ASSERT_TRUE(putResult == expected);
	}

	template<typename MessageType>
	std::shared_ptr<ghost::Reader<MessageType>> makeReader()
	{
		std::shared_ptr<ghost::Reader<MessageType>> res = _readable->getReader<MessageType>();
		EXPECT_TRUE(res);
		return res;
	}

	ghost::ConnectionConfiguration _config;
	std::shared_ptr<ghost::ReadableConnection> _readable;
	std::shared_ptr<ghost::internal::ReaderSink> _readerSink;

	std::shared_ptr<ghost::WritableConnection> _writable;
	std::shared_ptr<ghost::internal::WriterSink> _writerSink;

	google::protobuf::DoubleValue _doubleValue;
	static const double TEST_DOUBLE_VALUE;
};

const double ReaderWriterTests::TEST_DOUBLE_VALUE = 42;

/* Reader - ReaderSink - ReadableConnection */

TEST_F(ReaderWriterTests, test_ReadableConnection_messageGoesToReader_When_protobufMessageOfProperTypeIsPassedToSink)
{
	putToReadersink(_doubleValue);
	auto reader = makeReader<google::protobuf::DoubleValue>();

	google::protobuf::DoubleValue read;
	bool readResult = reader->read(read);
	ASSERT_TRUE(readResult);
}

TEST_F(ReaderWriterTests, test_ReadableConnection_messageGoesToReader_When_ghostMessageOfProperTypeIsPassedToSink)
{
	putToReadersink(*_ghostMessage);
	auto reader = makeReader<MessageMock>();

	bool readResult = reader->read(*_ghostMessage2);
	ASSERT_TRUE(readResult);
}

TEST_F(ReaderWriterTests, test_Reader_lastMessageIsAvailable_When_protobufMessageIsReadAndProperTypeIsRequested)
{
	putToReadersink(_doubleValue);
	auto reader = makeReader<google::protobuf::DoubleValue>();

	google::protobuf::DoubleValue read, readLast;
	reader->read(read);
	bool readLastResult = reader->lastRead(readLast);
	ASSERT_TRUE(readLastResult);
}

TEST_F(ReaderWriterTests, test_Reader_lastMessageIsAvailable_When_ghostMessageIsReadAndProperTypeIsRequested)
{
	putToReadersink(*_ghostMessage);
	auto reader = makeReader<MessageMock>();

	bool readResult = reader->read(*_ghostMessage2);
	bool readLastResult = reader->lastRead(*_ghostMessage2);
	ASSERT_TRUE(readLastResult);
}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_passedProtobufMessageTypeIsWrong)
{
	putToReadersink(_doubleValue);
	auto reader = makeReader<google::protobuf::Int32Value>();

	google::protobuf::Int32Value read;
	bool readResult = reader->read(read);
	ASSERT_FALSE(readResult);
}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_passedGhostMessageTypeIsWrong)
{
	putToReadersink(*_ghostMessage);
	auto reader = makeReader<MessageMock>();

	bool readResult = reader->read(*_otherTypeGhostMessage);
	ASSERT_FALSE(readResult);
}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_connectionWasNonBlockingAndNoMessageWasAvailable)
{
	_config.setOperationBlocking(false);
	setupReader();

	auto reader = makeReader<google::protobuf::DoubleValue>();
	google::protobuf::DoubleValue read;
	bool readResult = reader->read(read);
	ASSERT_FALSE(readResult);
}

TEST_F(ReaderWriterTests, test_ReaderSink_readFails_When_messageHandlerWasAdded)
{
	(void) _readable->addMessageHandler();

	putToReadersink(_doubleValue);
	auto reader = makeReader<google::protobuf::DoubleValue>();

	google::protobuf::DoubleValue read;
	bool readResult = reader->read(read);
	ASSERT_FALSE(readResult);
}

TEST_F(ReaderWriterTests, test_ReaderSink_readFails_When_sinkWasDrained)
{
	putToReadersink(*_ghostMessage);
	auto reader = makeReader<MessageMock>();

	_readerSink->drain();

	bool readResult = reader->read(*_ghostMessage2);
	ASSERT_FALSE(readResult);
}

TEST_F(ReaderWriterTests, test_ReaderSink_putToSinkFails_When_sinkWasDrained)
{
	_readerSink->drain();

	putToReadersink(*_ghostMessage, false);
}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerHandlesGhostMessage_When_messageHandlerWasAddedAndHandlesMessageTypeIsSent)
{
	auto messageHandler = _readable->addMessageHandler();
	int handlerCount = 0;
	messageHandler->addHandler<MessageMock>([&](const MessageMock& value) {
		handlerCount++;
		});

	putToReadersink(*_ghostMessage);
	ASSERT_TRUE(handlerCount == 1);
}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerHandlesProtobufMessage_When_messageHandlerWasAddedAndHandlesMessageTypeIsSent)
{
	auto messageHandler = _readable->addMessageHandler();
	int handlerCount = 0;
	messageHandler->addHandler<google::protobuf::DoubleValue>([&](const google::protobuf::DoubleValue& value) {
		handlerCount++;
		});

	putToReadersink(_doubleValue);
	ASSERT_TRUE(handlerCount == 1);
}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerDoesNotHandleMessage_When_noCorrespondingHandlerExists)
{
	auto messageHandler = _readable->addMessageHandler();
	int handlerCount = 0;
	messageHandler->addHandler<google::protobuf::Int32Value>([&](const google::protobuf::Int32Value& value) {
		handlerCount++;
		});

	putToReadersink(_doubleValue);
	ASSERT_TRUE(handlerCount == 0);
}

/* Writer - WriterSink - WritableConnection */

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_protobufMessageOfProperTypeIsPassedToWriter)
{
	auto writer = _writable->getWriter<google::protobuf::DoubleValue>();
	std::atomic_bool writeResult(false);
	std::thread t([&](){
		writeResult = writer->write(_doubleValue);
		});

	getFromWriterSink();
	t.join();
	ASSERT_TRUE(writeResult);
}

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_messageOfTypeAnyIsPassedToWriter)
{
	auto writer = _writable->getWriter<google::protobuf::Any>();
	google::protobuf::Any any;
	any.PackFrom(_doubleValue);
	std::atomic_bool writeResult(false);
	std::thread t([&]() {
		writeResult = writer->write(any);
		});

	getFromWriterSink();
	t.join();
	ASSERT_TRUE(writeResult);
}

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_ghostMessageOfProperTypeIsPassedToWriter)
{
	auto writer = _writable->getWriter<MessageMock>();
	std::atomic_bool writeResult(false);
	std::thread t([&]() {
		writeResult = writer->write(*_ghostMessage);
		});

	getFromWriterSink();
	t.join();
	ASSERT_TRUE(writeResult);
}

TEST_F(ReaderWriterTests, test_WriterSink_writeBlocks_When_connectionWasBlocking)
{
	auto writer = _writable->getWriter<google::protobuf::DoubleValue>();
	std::atomic_bool writeCompleted(false);
	std::thread t([&]() {
		writer->write(_doubleValue);
		writeCompleted = true;
		});

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	ASSERT_FALSE(writeCompleted);

	getFromWriterSink();
	t.join();
	ASSERT_TRUE(writeCompleted);
}

TEST_F(ReaderWriterTests, test_WriterSink_writeFails_When_sinkWasDrained)
{
	_config.setOperationBlocking(false);
	setupWriter();
	auto writer = _writable->getWriter<google::protobuf::DoubleValue>();

	_writerSink->drain();
	bool writeResult = writer->write(_doubleValue);
	ASSERT_FALSE(writeResult);
}

TEST_F(ReaderWriterTests, test_WriterSink_getFromSinkFails_When_sinkWasDrained)
{
	_config.setOperationBlocking(false);
	setupWriter();
	auto writer = _writable->getWriter<google::protobuf::DoubleValue>();

	bool writeResult = writer->write(_doubleValue);
	ASSERT_TRUE(writeResult);

	_writerSink->drain();
	getFromWriterSink(false);
}