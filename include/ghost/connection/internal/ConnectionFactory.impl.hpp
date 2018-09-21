template<typename Connection>
void ConnectionFactory::addServerRule(const ghost::ConnectionConfiguration& config)
{
	_serverRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addClientRule(const ghost::ConnectionConfiguration& config)
{
	_clientRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addPublisherRule(const ghost::ConnectionConfiguration& config)
{
	_publisherRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addSubscriberRule(const ghost::ConnectionConfiguration& config)
{
	_subscriberRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}
