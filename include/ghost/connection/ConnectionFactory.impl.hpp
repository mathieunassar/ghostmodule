template<typename Connection>
void ConnectionFactory::addServerRule(const ghost::ConnectionConfiguration& config)
{
	_internal->addServerRule<Connection>(config);
}

template<typename Connection>
void ConnectionFactory::addClientRule(const ghost::ConnectionConfiguration& config)
{
	_internal->addClientRule<Connection>(config);
}

template<typename Connection>
void ConnectionFactory::addPublisherRule(const ghost::ConnectionConfiguration& config)
{
	_internal->addPublisherRule<Connection>(config);
}

template<typename Connection>
void ConnectionFactory::addSubscriberRule(const ghost::ConnectionConfiguration& config)
{
	_internal->addSubscriberRule<Connection>(config);
}
