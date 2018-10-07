#ifndef GHOST_TESTS_CONNECTIONMOCKS_HPP
#define GHOST_TESTS_CONNECTIONMOCKS_HPP

#include <ghost/connection/Publisher.hpp>
#include <ghost/connection/Subscriber.hpp>
#include <ghost/connection/Client.hpp>
#include <ghost/connection/Server.hpp>

class PublisherMock : public ghost::Publisher
{
public:
	PublisherMock(const ghost::ConnectionConfiguration& config);
	virtual bool start() override;
	virtual bool stop() override;
	virtual bool isRunning() const override;

private:
	bool m_running;
};


class SubscriberMock : public ghost::Subscriber
{
public:
	SubscriberMock(const ghost::ConnectionConfiguration& config);
	virtual bool start() override;
	virtual bool stop() override;
	virtual bool isRunning() const override;

private:
	bool m_running;
};


class ClientMock : public ghost::Client
{
public:
	ClientMock(const ghost::ConnectionConfiguration& config);
	virtual bool start() override;
	virtual bool stop() override;
	virtual bool isRunning() const override;

private:
	bool m_running;
};

class ServerMock : public ghost::Server
{
public:
	ServerMock(const ghost::ConnectionConfiguration& config);
	virtual bool start() override;
	virtual bool stop() override;
	virtual bool isRunning() const override;

	virtual void setClientHandler(std::shared_ptr<ghost::ClientHandler> handler) override;

private:
	bool m_running;
};

#endif // GHOST_TESTS_CONNECTIONMOCKS_HPP