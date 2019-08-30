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

 /**
  *	This test class groups the following test categories:
  *	- MessageHandler
  *	- Reader, ReaderSink and ReadableConnection
  *	- Writer, WriterSink and WritableConnection
  */
class ReaderWriterTests : public testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}
};

/* Reader - ReaderSink - ReadableConnection */

TEST_F(ReaderWriterTests, test_ReadableConnection_messageGoesToReader_When_protobufMessageOfProperTypeIsPassedToSink)
{

}

TEST_F(ReaderWriterTests, test_ReadableConnection_messageGoesToReader_When_ghostMessageOfProperTypeIsPassedToSink)
{

}

TEST_F(ReaderWriterTests, test_Reader_lastMessageIsAvailable_When_protobufMessageIsReadAndProperTypeIsRequested)
{

}

TEST_F(ReaderWriterTests, test_Reader_lastMessageFails_When_protobufMessageIsReadAndWrongTypeIsRequested)
{

}

TEST_F(ReaderWriterTests, test_Reader_lastMessageIsAvailable_When_ghostMessageIsReadAndProperTypeIsRequested)
{

}

TEST_F(ReaderWriterTests, test_Reader_lastMessageFails_When_ghostMessageIsReadAndWrongTypeIsRequested)
{

}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_passedProtobufMessageTypeIsWrong)
{

}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_passedGhostMessageTypeIsWrong)
{

}

TEST_F(ReaderWriterTests, test_Reader_readFails_When_connectionWasBlockingAndNoMessageWasAvailable)
{

}

TEST_F(ReaderWriterTests, test_ReaderSink_readFails_When_messageHandlerWasAdded)
{

}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerHandlesGhostMessage_When_messageHandlerWasAddedAndHandlesMessageTypeIsSent)
{

}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerHandlesProtobufMessage_When_messageHandlerWasAddedAndHandlesMessageTypeIsSent)
{

}

TEST_F(ReaderWriterTests, test_ReaderSink_messageHandlerDoesNotHandleMessage_When_noCorrespondingHandlerExists)
{

}

/* Writer - WriterSink - WritableConnection */

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_protobufMessageOfProperTypeIsPassedToWriter)
{

}

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_messageOfTypeAnyIsPassedToWriter)
{

}

TEST_F(ReaderWriterTests, test_WritableConnection_messageGoesToSink_When_ghostMessageOfProperTypeIsPassedToWriter)
{

}

TEST_F(ReaderWriterTests, test_Writer_writeFails_When_passedProtobufMessageTypeIsWrong)
{
	// this will probably hard to get to fail
}

TEST_F(ReaderWriterTests, test_Writer_writeFails_When_passedGhostMessageTypeIsWrong)
{
	// this will probably hard to get to fail
}

TEST_F(ReaderWriterTests, test_WriterSink_writeBlocks_When_connectionWasBlocking)
{

}
