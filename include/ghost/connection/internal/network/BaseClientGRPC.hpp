#ifndef GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP

#include <ghost/connection/Client.hpp>

#include <string>
#include <deque>
#include <atomic>
#include <memory>
#include <mutex>

#include <ghost/connection/internal/ReaderSink.hpp>
#include <ghost/connection/internal/WriterSink.hpp>
#include <ghost/connection/internal/GenericMessageConverter.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/internal/network/RPCStateMachine.hpp>

#include <ghost/connection/MessageHandler.hpp>
#include <ghost/connection/ClientHandler.hpp>

namespace ghost
{
	namespace internal
	{
		static const std::string BASE_CLIENT_GRPC_CONFIG_NOWRITER = "BASE_CLIENT_GRPC_CONFIG_NOWRITER";

		template<typename ReaderWriter, typename ContextType>
		class BaseClientGRPC : public ghost::Client, public std::enable_shared_from_this<BaseClientGRPC<ReaderWriter, ContextType>>
		{
		public:
			BaseClientGRPC(const ghost::NetworkConnectionConfiguration& config, grpc::CompletionQueue* completionQueue);
			virtual ~BaseClientGRPC() = 0;

			bool start() override;
			bool stop() override;
			bool isRunning() const override;
			bool isFinished() const;

		protected:
			bool isWriterConfigured() const;
			void startReader();
			void startWriter();

			/* async operations management */
			std::atomic<int> _operationsRunning;
			void startOperation();
			bool finishOperation();
			void awaitFinished();

			/* Write operations */
			std::thread _writerThread;
			std::mutex _writerMutex;
			std::condition_variable _writerConditionVariable;
			bool _writeInProgress;
			std::function<void(bool)> _writtenProcessor;
			void onWriteFinished(bool ok);

			/* Read operations */
			google::protobuf::Any _incomingMessage;
			std::function<void(bool)> _readProcessor;
			void onReadFinished(bool ok);

			/* gRPC and connection objects */
			ghost::NetworkConnectionConfiguration _configuration;
			RPCStateMachine _statemachine;
			grpc::CompletionQueue* _completionQueue;
			std::unique_ptr<ReaderWriter> _client;
			std::unique_ptr<ContextType> _context;
			void disposeGRPC();
		};

		#include "BaseClientGRPC.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP
