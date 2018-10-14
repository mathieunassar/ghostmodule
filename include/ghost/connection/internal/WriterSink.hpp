#ifndef GHOST_INTERNAL_WRITERSINK_HPP
#define GHOST_INTERNAL_WRITERSINK_HPP

#include <google/protobuf/any.pb.h>
#include <ghost/connection/internal/QueuedSink.hpp>

namespace ghost
{
	namespace internal
	{
		/*
		This will contain the write queue, constructor can take another writer sink to transfer the write queue
		*/
		class WriterSink : public QueuedSink
		{
		public:
			virtual ~WriterSink() = 0;

			/**
			* Gets a message from the writer.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	message	message to send.
			* @return	A long.
			*/
			virtual bool get(google::protobuf::Any& message, std::chrono::milliseconds timeout) = 0;

			/**
			 *	Remove the last message from the queue. This method must be called after the message
			 *	is effectively sent.
			 */
			virtual void pop() = 0;
		};

		inline WriterSink::~WriterSink() {}
	}
}

#endif //GHOST_INTERNAL_WRITERSINK_HPP
