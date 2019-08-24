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

#ifndef GHOST_INTERNAL_NETWORK_RPC_HPP
#define GHOST_INTERNAL_NETWORK_RPC_HPP

#include <atomic>
#include <memory>

#include <ghost/connection_grpc/ServerClientService.pb.h>
#include <ghost/connection_grpc/ServerClientService.grpc.pb.h>

#include "RPCStateMachine.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	Container for runtime information about the RPC call.
		 *	This class contains the call's state machine, the necessary gRPC objects as well
		 *	as the list of operations currently active.
		 */
		template<typename ReaderWriter, typename ContextType>
		class RPC
		{
		public:
			RPC();
			~RPC();

			/// Clears gRPC objects. Call this before the destructor.
			void disposeGRPC();

			/* RPC State management */
			/// Transitions from CREATED to INITIALIZING.
			bool initialize();
			/// Transitions from EXECUTING or INACTIVE to DISPOSING.
			bool dispose();
			/// Increases the number of ongoing operations by one.
			void startOperation();
			/// Decreases the number of ongoing operations by one, and returns false if the RPC completed.
			bool finishOperation();

			/* RPC state accessors */
			/// Checks that the RPC is completed and has no operation ongoing.
			bool isFinished() const;
			/// Blocks until no more operations are ongoing.
			void awaitFinished();

			/* Object accessors */
			/// @return the state machine of this RPC.
			const RPCStateMachine& getStateMachine() const;
			RPCStateMachine& getStateMachine();
			/// Sets this RPC's connection.
			void setClient(std::unique_ptr<ReaderWriter>&& client);
			/// @return the client handle.
			const std::unique_ptr<ReaderWriter>& getClient() const;
			/// @return this RPC's context.
			const std::unique_ptr<ContextType>& getContext() const;

		protected:
			/* async operations management */
			std::atomic<int> _operationsRunning;

			/* gRPC and connection objects */
			RPCStateMachine _statemachine;
			std::unique_ptr<ReaderWriter> _client;
			std::unique_ptr<ContextType> _context;

		};

		#include "RPC.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_RPC_HPP
