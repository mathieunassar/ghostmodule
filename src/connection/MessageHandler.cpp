#include "../include/ghost/connection/internal/MessageHandler.hpp"
#include "../include/ghost/connection/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

MessageHandler::MessageHandler()
{
	_internal = this;
}

void MessageHandler::handle(const google::protobuf::Any& message)
{
	std::pair<std::string, std::string> formatAndName = GenericMessageConverter::getFormatAndName(message);

	if (_handlers.count(formatAndName))
	{
		_handlers.at(formatAndName)->handle(message);
	}
}
