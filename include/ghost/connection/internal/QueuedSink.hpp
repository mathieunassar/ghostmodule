#ifndef GHOST_INTERNAL_QUEUEDSINK_HPP
#define GHOST_INTERNAL_QUEUEDSINK_HPP

#include <google/protobuf/any.pb.h>
#include <BlockingQueue.hpp>

namespace ghost
{
	namespace internal
	{
		class QueuedSink
		{
		public:
			QueuedSink()
				: _messageQueue(new BlockingQueue<google::protobuf::Any>()) {}
			virtual ~QueuedSink() = 0;

		protected:
			std::shared_ptr<BlockingQueue<google::protobuf::Any>> getMessageQueue()
			{
				return _messageQueue;
			}

		private:
			std::shared_ptr<BlockingQueue<google::protobuf::Any>> _messageQueue;
		};

		inline QueuedSink::~QueuedSink() {}
	}
}

#endif //GHOST_INTERNAL_QUEUEDSINK_HPP
