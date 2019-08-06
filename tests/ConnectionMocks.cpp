/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
