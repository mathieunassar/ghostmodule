template<typename MessageType>
PublisherGRPC<MessageType>::PublisherGRPC(const ghost::NetworkConnectionConfiguration& config)
	: _server(config)
{
	_handler = std::make_shared<PublisherClientHandler>();
	_server.setClientHandler(_handler);
}

template<typename MessageType>
bool PublisherGRPC<MessageType>::start()
{
	return _server.start();
}

template<typename MessageType>
bool PublisherGRPC<MessageType>::stop()
{
	return _server.stop();
}

template<typename MessageType>
bool PublisherGRPC<MessageType>::isRunning() const
{
	return _server.isRunning();
}

template<typename MessageType>
void PublisherGRPC<MessageType>::publish(const MessageType& message)
{
	auto sh = std::make_shared<MessageType>(message);
	_handler->send(ghost::ProtobufMessage(sh));
}
