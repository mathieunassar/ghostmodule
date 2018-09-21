template<typename MessageType>
GenericReader<MessageType>::GenericReader(bool blocking)
	: _blocking(blocking)
{

}

template<typename MessageType>
GenericReader<MessageType>::GenericReader(const ReaderSink& other, bool blocking)
	: ReaderSink(other)
	, _blocking(blocking)
{

}

template<typename MessageType>
bool GenericReader<MessageType>::put(const google::protobuf::Any& message)
{
	if (_messageHandler) // if there is a message handler, don't use the read queue
		_messageHandler->handle(message);
	else
		getMessageQueue()->push(message);

	return true;
}

template<typename MessageType>
bool GenericReader<MessageType>::read(MessageType& message)
{
	if (_messageHandler)
		return false; // a message handler is configured, this method cannot be called

	// call is non blocking and there is nothing to read, return false.
	// User can find out that there is no issue by calling "isRunnung()" on the connection
	if (!_blocking && getMessageQueue()->size() == 0)
		return false;

	// else, connection is blocking or there is something to read
	_last = getMessageQueue()->get(); // copy is called here
	getMessageQueue()->pop();

	bool parseResult = makeMessage(_last, message);

	return parseResult;
}

template<typename MessageType>
bool GenericReader<MessageType>::lastRead(MessageType& message)
{
	bool parseResult = makeMessage(_last, message);

	return parseResult;
}

template<typename MessageType>
std::shared_ptr<ghost::MessageHandler> GenericReader<MessageType>::addMessageHandler()
{
	_messageHandler = std::make_shared<MessageHandler>();
	return _messageHandler;
}
