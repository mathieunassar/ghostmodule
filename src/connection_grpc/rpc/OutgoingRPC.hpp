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

#include <grpcpp/client_context.h>

#include <memory>

#include "../CompletionQueueExecutor.hpp"
#include "RPC.hpp"
#include "ReaderRPC.hpp"
#include "WriterRPC.hpp"

namespace ghost
{
namespace internal
{
// - give the operations an interface which can be faked
// - RPCRead, RPCReadOne, RPCWrite, RPCWriteOne, RPCConnect, RPCFinsh

class OutgoingRPC
    : public ReaderRPC<grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>,
		       grpc::ClientContext>,
      public WriterRPC<grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ClientContext>
{
public:
	using ReaderWriter = grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>;
	using ContextType = grpc::ClientContext;

	OutgoingRPC(const std::shared_ptr<ghost::ThreadPool>& threadPool, const std::string& serverIp, int serverPort,
		    size_t dedicatedThreads);
	~OutgoingRPC();

	bool start();
	bool stop();
	bool isRunning() const;

	// configuration: set a writer sink or a reader sink to activate the feature
	void setWriterSink(const std::shared_ptr<ghost::WriterSink>& sink);
	void setReaderSink(const std::shared_ptr<ghost::ReaderSink>& sink);

private:
	void onRPCStateChanged(RPCStateMachine::State newState);
	void dispose();

	std::shared_ptr<ghost::ThreadPool> _threadPool;
	grpc::CompletionQueue* _completionQueue;
	std::shared_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub> _stub;

	std::string _serverIp;
	int _serverPort;

	std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
	CompletionQueueExecutor _executor;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_OUTGOINGRPC_HPP
