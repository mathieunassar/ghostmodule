template<typename MessageType>
SubscriberGRPC<MessageType>::SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config)
	: _client(config)
{

}

template<typename MessageType>
bool SubscriberGRPC<MessageType>::start()
{
	return _client.start();
}

template<typename MessageType>
bool SubscriberGRPC<MessageType>::stop()
{
	return _client.stop();
}

template<typename MessageType>
bool SubscriberGRPC<MessageType>::isRunning() const
{
	return _client.isRunning();
}

template<typename MessageType>
void SubscriberGRPC<MessageType>::setMessageHandler(std::function<void(const MessageType& message)> handler)
{
	auto msgHandler =_client.addMessageHandler();
	msgHandler->addHandler(handler);
}
