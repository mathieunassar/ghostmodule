#ifndef GHOST_INTERNAL_CONNECTION_HPP
#define GHOST_INTERNAL_CONNECTION_HPP

#include <memory>

#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/internal/GenericWriter.hpp>
#include <ghost/connection/internal/GenericReader.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		* The base class for any connection.
		* Handles the writer and reader sinks.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		*/
		class Connection
		{
		public:
			Connection(const ConnectionConfiguration& configuration);
			virtual ~Connection() = 0;

			template<typename MessageType>
			std::shared_ptr<Writer<MessageType>> getWriter();

			template<typename MessageType>
			std::shared_ptr<Reader<MessageType>> getReader();

		protected:
			/// a new sink is available - default implementation: replace the member variable
			virtual void setWriterSink(std::shared_ptr<WriterSink> sink);
			/// a new sink is available - default implementation: replace the member variable
			virtual void setReaderSink(std::shared_ptr<ReaderSink> sink);
			/// returns a shared_ptr to the currently configured writer sink
			std::shared_ptr<WriterSink> getWriterSink();
			/// returns a shared_ptr to the currently configured reader sink
			std::shared_ptr<ReaderSink> getReaderSink();

			/// copies the current reader sink to the other connection
			void forwardReaderSink(Connection& to);
			/// copies the current writer sink to the other connection
			void forwardWriterSink(Connection& to);

		private:
			ConnectionConfiguration _configuration;
			std::shared_ptr<WriterSink> _writer;
			std::shared_ptr<ReaderSink> _reader;
		};

		inline Connection::~Connection() {}

		#include "Connection.impl.hpp"
	}
}

#endif //GHOST_CONNECTION_HPP
