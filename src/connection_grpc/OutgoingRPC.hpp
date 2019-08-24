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

#ifndef GHOST_INTERNAL_NETWORK_OUTGOINGRPC_HPP
#define GHOST_INTERNAL_NETWORK_OUTGOINGRPC_HPP

#include <memory>
#include <grpcpp/client_context.h>

#include "RPC.hpp"
#include "CompletionQueueExecutor.hpp"
#include "RPCRead.hpp"
#include "RPCWrite.hpp"

namespace ghost
{
	namespace internal
	{
		// - give the operations an interface which can be faked
		// - RPCRead, RPCReadOne, RPCWrite, RPCWriteOne, RPCConnect, RPCFinsh
		
		class OutgoingRPC
		{
		public:
			using ReaderWriter = grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>;
			using ContextType = grpc::ClientContext;

			OutgoingRPC(const std::string& serverIp, int serverPort, size_t dedicatedThreads);

			bool start();
			bool stop();
			bool isRunning() const;

			// configuration: set a writer sink or a reader sink to activate the feature
			void setWriterSink(const std::shared_ptr<ghost::WriterSink>& sink);
			void setReaderSink(const std::shared_ptr<ghost::ReaderSink>& sink);

		private:
			grpc::CompletionQueue* _completionQueue;
			std::shared_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub> _stub;

			std::string _serverIp;
			int _serverPort;
			std::shared_ptr<ghost::ReaderSink> _readerSink;
			std::shared_ptr<ghost::WriterSink> _writerSink;

			std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
			std::shared_ptr<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>> _writerOperation;
			std::shared_ptr<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>> _readerOperation;
			CompletionQueueExecutor _executor;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_OUTGOINGRPC_HPP
