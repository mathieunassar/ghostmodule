#include "../../include/internal/network/PublisherGRPC.hpp"
#include "../../include/internal/network/PublisherClientHandler.hpp"

using namespace ghost::internal;

PublisherGRPC::PublisherGRPC(const ghost::ConnectionConfiguration& config)
	: PublisherGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config))
{

}

PublisherGRPC::PublisherGRPC(const ghost::NetworkConnectionConfiguration& config)
	: Publisher(config)
	, _server(config)
	, _writerThreadEnable(false)
{
	_handler = std::make_shared<PublisherClientHandler>();
	_server.setClientHandler(_handler);
}

bool PublisherGRPC::start()
{
	_writerThreadEnable = true;
	_writerThread = std::thread(&PublisherGRPC::writerThread, this);

	return _server.start();
}

bool PublisherGRPC::stop()
{
	_writerThreadEnable = false;
	if (_writerThread.joinable())
		_writerThread.join();

	_handler->releaseClients();
	return _server.stop();
}

bool PublisherGRPC::isRunning() const
{
	return _server.isRunning();
}

void PublisherGRPC::writerThread()
{
	while (_writerThreadEnable)
	{
		auto writer = getWriterSink();
		google::protobuf::Any message;
		bool getSuccess = writer->get(message, std::chrono::milliseconds(10));

		if (getSuccess)
		{
			_handler->send(message);
			writer->pop();
		}
	}
}
