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

#ifndef GHOST_INTERNAL_NETWORK_INCOMINGRPC_HPP
#define GHOST_INTERNAL_NETWORK_INCOMINGRPC_HPP

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../RemoteClientGRPC.hpp"
#include "RPC.hpp"
#include "RPCRead.hpp"
#include "RPCWrite.hpp"
#include "RPCRequest.hpp"
#include "RPCServerFinish.hpp"
#include "RPCDone.hpp"

namespace ghost
{
	namespace internal
	{
		class IncomingRPC
		{
		public:
			using ReaderWriter = grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>;
			using ContextType = grpc::ServerContext;
			using ServiceType = ghost::protobuf::connectiongrpc::ServerClientService::AsyncService;

			IncomingRPC(ghost::protobuf::connectiongrpc::ServerClientService::AsyncService* service,
				grpc::ServerCompletionQueue* completionQueue,
				const std::function<void(std::shared_ptr<RemoteClientGRPC>)>& clientConnectedCallback);
			~IncomingRPC();

			bool start();
			void startWriter(const std::shared_ptr<ghost::WriterSink>& sink);
			void startReader(const std::shared_ptr<ghost::ReaderSink>& sink);
			bool stop(const grpc::Status& status = grpc::Status::OK);

			void dispose();

			bool isFinished() const;

			void setParent(std::weak_ptr<RemoteClientGRPC> parent);
			std::shared_ptr<RemoteClientGRPC> getParent();

		protected:
			void onRPCConnected();
			std::function<void(std::shared_ptr<RemoteClientGRPC>)> _serverCallback;
			
			std::weak_ptr<RemoteClientGRPC> _parent;
			std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
			std::shared_ptr<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>> _writerOperation;
			std::shared_ptr<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>> _readerOperation;
			std::shared_ptr<RPCRequest<ReaderWriter, ContextType, ServiceType>> _requestOperation;
			std::shared_ptr<RPCServerFinish<ReaderWriter, ContextType>> _finishOperation;
			std::shared_ptr<RPCDone<ReaderWriter, ContextType>> _doneOperation;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_INCOMINGRPC_HPP
