#include "ConnectionMocks.hpp"

PublisherMock::PublisherMock(const ghost::ConnectionConfiguration& config)
	: Publisher(config)
	, m_running(false)
{

}

bool PublisherMock::start()
{
	m_running = true;
	return true;
}

bool PublisherMock::stop()
{
	m_running = false;
	return true;
}

bool PublisherMock::isRunning() const
{
	return m_running;
}

SubscriberMock::SubscriberMock(const ghost::ConnectionConfiguration& config)
	: Subscriber(config)
	, m_running(false)
{

}

bool SubscriberMock::start()
{
	m_running = true;
	return true;
}

bool SubscriberMock::stop()
{
	m_running = false;
	return true;
}

bool SubscriberMock::isRunning() const
{
	return m_running;
}


ClientMock::ClientMock(const ghost::ConnectionConfiguration& config)
	: Client(config)
	, m_running(false)
{

}

bool ClientMock::start()
{
	m_running = true;
	return true;
}

bool ClientMock::stop()
{
	m_running = false;
	return true;
}

bool ClientMock::isRunning() const
{
	return m_running;
}


ServerMock::ServerMock(const ghost::ConnectionConfiguration& config)
	: Server(config)
	, m_running(false)
{

}

bool ServerMock::start()
{
	m_running = true;
	return true;
}

bool ServerMock::stop()
{
	m_running = false;
	return true;
}

bool ServerMock::isRunning() const
{
	return m_running;
}

void ServerMock::setClientHandler(std::shared_ptr<ghost::ClientHandler> handler)
{
	// nothing here
}
