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

#ifndef GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include <mutex>
#include <condition_variable>
#include <grpcpp/client_context.h>
#include "BaseClientGRPC.hpp"
#include "CompletionQueueExecutor.hpp"

namespace ghost
{
	namespace internal
	{
		class ClientGRPC : public BaseClientGRPC<grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ClientContext>
		{
		public:
			ClientGRPC(const ghost::ConnectionConfiguration& config);
			ClientGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;

		protected:
			void onStarted(bool ok);
			void onFinished(bool ok);

			std::function<void(bool)> _startedProcessor;
			std::function<void(bool)> _finishProcessor;
			std::unique_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub> _stub;
			CompletionQueueExecutor _executor;

			std::mutex _initializedMutex;
			std::condition_variable _initializedConditionVariable;
			bool _initialized;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
