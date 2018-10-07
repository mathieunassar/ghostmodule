template<typename MessageType>
GenericWriter<MessageType>::GenericWriter(bool blocking)
	: _blocking(blocking)
{

}

template<typename MessageType>
GenericWriter<MessageType>::GenericWriter(const WriterSink& other, bool blocking)
	: WriterSink(other)
	, _blocking(blocking)
{

}

template<typename MessageType>
bool GenericWriter<MessageType>::get(google::protobuf::Any& message, std::chrono::milliseconds timeout)
{
	if (timeout == std::chrono::milliseconds(0))
	{
		message = getMessageQueue()->get();
		return true;
	}

	return getMessageQueue()->tryGet(timeout, message);
}

template<typename MessageType>
void GenericWriter<MessageType>::pop()
{
	getMessageQueue()->pop();
}

template<typename MessageType>
bool GenericWriter<MessageType>::write(const MessageType& message)
{
	google::protobuf::Any any;

	bool createSuccess = makeAny(any, message);

	if (!createSuccess)
	{
		return false; // conversion failed, i.e. there is no protobuf message in the input or the serialization failed (in case of a user format)
	}

	getMessageQueue()->push(any);

	if (_blocking)
	{
		while (getMessageQueue()->size() != 0) // blocks until the message is gone, use blocking queue instead!
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	return true;
}
