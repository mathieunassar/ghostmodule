#include "../../include/ghost/connection/internal/network/SubscriberGRPC.hpp"

using namespace ghost::internal;

SubscriberGRPC::SubscriberGRPC(const ghost::ConnectionConfiguration& config)
	: SubscriberGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config))
{

}

SubscriberGRPC::SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config)
	: Subscriber(config)
	, _client(makeNoWriterConfig(config))
{

}

bool SubscriberGRPC::start()
{
	return _client.start();
}

bool SubscriberGRPC::stop()
{
	return _client.stop();
}

bool SubscriberGRPC::isRunning() const
{
	return _client.isRunning();
}

void SubscriberGRPC::setReaderSink(std::shared_ptr<ReaderSink> sink)
{
	internal::Connection::setReaderSink(sink);
	forwardReaderSink(_client);
}

ghost::NetworkConnectionConfiguration SubscriberGRPC::makeNoWriterConfig(const ghost::NetworkConnectionConfiguration& config)
{
	ghost::NetworkConnectionConfiguration newConfig(config);
	newConfig.addAttribute<bool>(BASE_CLIENT_GRPC_CONFIG_NOWRITER, true, true);
	return newConfig;
}
