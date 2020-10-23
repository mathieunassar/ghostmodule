/*
 * Copyright 2020 Mathieu Nassar
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

#include <functional>
#include <ghost/connection/ReaderSink.hpp>
#include <ghost/connection/WriterSink.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>

#include "RPC.hpp"
#include "RPCDone.hpp"
#include "RPCRequest.hpp"
#include "RPCServerFinish.hpp"
#include "ReaderRPC.hpp"
#include "WriterRPC.hpp"

namespace ghost
{
namespace internal
{
class RemoteClientGRPC;

/**
 *	Manages gRPC calls for an incoming connection (a client connection to this server).
 *	This object is created by ghost::internal::ServerGRPC (and therefore also by ghost::internal::PublisherGRPC).
 */
class IncomingRPC
    : public ReaderRPC<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>,
		       grpc::ServerContext>,
      public WriterRPC<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ServerContext>
{
public:
	using ReaderWriter = grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>;
	using ContextType = grpc::ServerContext;
	using ServiceType = ghost::protobuf::connectiongrpc::ServerClientService::AsyncService;

	IncomingRPC(ghost::protobuf::connectiongrpc::ServerClientService::AsyncService* service,
		    grpc::ServerCompletionQueue* completionQueue, const std::shared_ptr<ghost::ThreadPool>& threadPool,
		    const std::function<void(std::shared_ptr<RemoteClientGRPC>)>& clientConnectedCallback);
	~IncomingRPC();

	bool start();
	bool stop(const grpc::Status& status = grpc::Status::OK);

	void dispose();

	bool isFinished() const;

	void setParent(std::weak_ptr<RemoteClientGRPC> parent);
	std::shared_ptr<RemoteClientGRPC> getParent();

private:
	void onRPCConnected();
	void onRPCStateChanged(RPCStateMachine::State newState);
	std::function<void(std::shared_ptr<RemoteClientGRPC>)> _serverCallback;

	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::weak_ptr<RemoteClientGRPC> _parent;
	std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
	std::shared_ptr<RPCRequest<ReaderWriter, ContextType, ServiceType>> _requestOperation;
	std::shared_ptr<RPCServerFinish<ReaderWriter, ContextType>> _finishOperation;
	std::shared_ptr<RPCDone<ReaderWriter, ContextType>> _doneOperation;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_INCOMINGRPC_HPP
