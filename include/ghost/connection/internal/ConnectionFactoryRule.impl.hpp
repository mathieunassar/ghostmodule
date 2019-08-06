template<typename Connection>
ConnectionFactoryGenericRule<Connection>::ConnectionFactoryGenericRule(const ghost::ConnectionConfiguration& minimumConfiguration)
	: ConnectionFactoryRule(minimumConfiguration)
{

}

template<typename Connection>
std::shared_ptr<ghost::Connection> ConnectionFactoryGenericRule<Connection>::create(const ghost::ConnectionConfiguration& config) const
{
	return std::make_shared<Connection>(config);
}
