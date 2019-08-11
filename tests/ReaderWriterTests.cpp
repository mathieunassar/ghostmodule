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
//
//#include <iostream>
//#include <gtest/gtest.h>
//
//#include <ghost/connection/internal/GenericReader.hpp>
//#include <ghost/connection/internal/GenericWriter.hpp>
//
//#include "../protobuf/Ghost.pb.h"
//
//using namespace ghost::internal;
//
//class ReaderWriterTests : public testing::Test
//{
//protected:
//	void SetUp() override
//	{
//
//	}
//
//	void TearDown() override
//	{
//
//	}
//};
//
//TEST_F(ReaderWriterTests, test_genericreader_proto)
//{
//	GenericReader<protobuf::GenericMessageHeader> reader(false);
//
//	// create a test message
//	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
//	proto->set_timestamp(582);
//
//	// convert it to any
//	google::protobuf::Any anymessage;
//	anymessage.PackFrom(*proto);
//
//	// feed the reader
//	reader.put(anymessage);
//
//	// check that you can read it now
//	protobuf::GenericMessageHeader newmessage;
//	bool readSuccess = reader.read(newmessage);
//	ASSERT_TRUE(readSuccess);
//	ASSERT_TRUE(newmessage.timestamp() == 582);
//
//	// read again, should return false because the call is non blocking
//	bool readSuccess2 = reader.read(newmessage);
//	ASSERT_TRUE(!readSuccess2);
//
//	// add it again...
//	reader.put(anymessage);
//
//	// copy the reader
//	GenericReader<protobuf::GenericMessageHeader> reader2(reader, false);
//
//	// normally it is possible to read from it
//	protobuf::GenericMessageHeader newmessage2;
//	bool readSuccess3 = reader2.read(newmessage2);
//	ASSERT_TRUE(readSuccess3);
//	ASSERT_TRUE(newmessage2.timestamp() == 582);
//
//	// and reading from the old one is now finished (they share the queue)
//	bool readSuccess4 = reader.read(newmessage2);
//	ASSERT_TRUE(!readSuccess4);
//}
//
//TEST_F(ReaderWriterTests, test_genericWriter_any)
//{
//	GenericWriter<google::protobuf::Any> writer(false);
//
//	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
//	proto->set_timestamp(582);
//	google::protobuf::Any anymessage;
//	anymessage.PackFrom(*proto);
//
//	bool writeSuccess = writer.write(anymessage);
//	ASSERT_TRUE(writeSuccess);
//}
//
//TEST_F(ReaderWriterTests, test_genericwriter_proto)
//{
//	GenericWriter<protobuf::GenericMessageHeader> writer(false);
//
//	// create a test message
//	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
//	proto->set_timestamp(582);
//
//	bool writeSuccess = writer.write(*proto);
//	ASSERT_TRUE(writeSuccess);
//
//	google::protobuf::Any anymessage;
//	bool getSuccess = writer.get(anymessage);
//	ASSERT_TRUE(getSuccess);
//
//	protobuf::GenericMessageHeader newmessage;
//	bool unpackSuccess = anymessage.UnpackTo(&newmessage);
//	ASSERT_TRUE(unpackSuccess);
//	ASSERT_TRUE(newmessage.timestamp() == 582);
//}
//
//TEST_F(ReaderWriterTests, test_genericreader_ghost)
//{
//	GenericReader<ghost::ProtobufMessage> reader(false);
//
//	// create a test message
//	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
//	proto->set_timestamp(582);
//	ghost::ProtobufMessage message(proto);
//
//	// convert it to any
//	google::protobuf::Any anymessage;
//	anymessage.PackFrom(*proto);
//
//	reader.put(anymessage);
//
//	// check that it is readable
//	ghost::ProtobufMessage message2(std::make_shared<protobuf::GenericMessageHeader>());
//	bool readSuccess = reader.read(message2);
//	ASSERT_TRUE(readSuccess);
//}
//
//TEST_F(ReaderWriterTests, test_genericwriter_ghost)
//{
//	GenericWriter<ghost::ProtobufMessage> writer(false);
//
//	// create a test message
//	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
//	proto->set_timestamp(582);
//	ghost::ProtobufMessage message(proto);
//
//	bool writeSuccess = writer.write(message);
//	ASSERT_TRUE(writeSuccess);
//
//	google::protobuf::Any anymessage;
//	bool getSuccess = writer.get(anymessage);
//	ASSERT_TRUE(getSuccess);
//}
