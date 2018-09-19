#ifndef GHOST_INTERNAL_READERSINK_HPP
#define GHOST_INTERNAL_READERSINK_HPP

#include <google/protobuf/any.pb.h>
#include <BlockingQueue.hpp>
#include "QueuedSink.hpp"

namespace ghost
{
	namespace internal
	{
		/* 
		This will contain the read queue, will be initialized at the beginning of a connection
		constructor can take another sink to transfer the queue from one to another
		Create MessageQueue class to handle the queue with its mutex
		*/
		class ReaderSink : public QueuedSink
		{
		public:
			virtual ~ReaderSink() = 0 {}

			/**
			* Puts a message in the reader.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	message	message to send.
			* @return	A long.
			*/
			virtual bool put(const google::protobuf::Any& message) = 0;
		};
	}
}

#endif //GHOST_INTERNAL_READERSINK_HPP
