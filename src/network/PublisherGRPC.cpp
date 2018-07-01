#include "../../include/internal/network/PublisherGRPC.hpp"
#include "../../include/internal/network/PublisherClientHandler.hpp"

using namespace ghost::internal;

template<>
void PublisherGRPC<ghost::Message>::publish(const ghost::Message& message)
{
	_handler->send(message);
}
