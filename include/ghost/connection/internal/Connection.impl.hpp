template<typename MessageType>
std::shared_ptr<Writer<MessageType>> Connection::getWriter()
{
	// create a new writer with the message queue of the existing one - _writer is not nullptr because it is initialized in the constructor
	auto newWriter = std::make_shared<GenericWriter<MessageType>>(*_writer, _configuration.isOperationBlocking());
	setWriterSink(newWriter);
	return newWriter;
}

template<typename MessageType>
std::shared_ptr<Reader<MessageType>> Connection::getReader()
{
	// create a new reader with the message queue of the existing one - _reader is not nullptr because it is initialized in the constructor
	auto newReader = std::make_shared<GenericReader<MessageType>>(*_reader, _configuration.isOperationBlocking());
	setReaderSink(newReader);
	return newReader;
}
