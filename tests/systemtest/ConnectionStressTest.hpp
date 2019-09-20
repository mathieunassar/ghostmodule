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

#ifndef GHOST_TESTS_CONNECTIONSTRESSTEST_HPP
#define GHOST_TESTS_CONNECTIONSTRESSTEST_HPP

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/connection/Writer.hpp>
#include <ghost/connection/Reader.hpp>
#include <google/protobuf/wrappers.pb.h>
#include "Systemtest.hpp"

class ConnectionStressTest : public Systemtest
{
public:
	ConnectionStressTest(const std::shared_ptr<ghost::Logger>& logger);

	bool setUp() override;
	bool run() override;
	std::string getName() const override;

private:
	static const std::string TEST_NAME;

	bool messageHandler(const google::protobuf::StringValue& message, size_t subscriberId);

	std::shared_ptr<ghost::ConnectionManager> _connectionManager;

	std::shared_ptr<ghost::Writer<google::protobuf::StringValue>> _publisherWriter;
	long long _messageSentIndex;
	std::vector<long long> _messageReceivedIndex;
};

#endif // GHOST_TESTS_CONNECTIONSTRESSTEST_HPP
