#ifndef GHOST_INTERNAL_CONNECTION_FACTORY_HPP
#define GHOST_INTERNAL_CONNECTION_FACTORY_HPP

#include <memory>
#include <list>

#include "ConnectionFactoryRule.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Publisher.hpp"
#include "Subscriber.hpp"
#include "ConnectionConfiguration.hpp"

#include "../ConnectionFactory.hpp"


namespace ghost
{
	namespace internal
	{
		class ConnectionFactory : public ghost::ConnectionFactory
		{
		public:
			ConnectionFactory();

			/* Methods to set up the factory */

			template<typename Connection>
			void addServerRule(const ghost::ConnectionConfiguration& config);

			template<typename Connection>
			void addClientRule(const ghost::ConnectionConfiguration& config);

			template<typename Connection>
			void addPublisherRule(const ghost::ConnectionConfiguration& config);
			
			template<typename Connection>
			void addSubscriberRule(const ghost::ConnectionConfiguration& config);

			/* Methods to create connections */

			std::shared_ptr<ghost::Server> createServer(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Client> createClient(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Publisher> createPublisher(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Subscriber> createSubscriber(const ghost::ConnectionConfiguration& config);

		private:
			std::list<std::unique_ptr<ConnectionFactoryRule>> _serverRules;
			std::list<std::unique_ptr<ConnectionFactoryRule>> _clientRules;
			std::list<std::unique_ptr<ConnectionFactoryRule>> _publisherRules;
			std::list<std::unique_ptr<ConnectionFactoryRule>> _subscriberRules;
		};

		#include "internal/ConnectionFactory.impl.hpp"
	}
}

#endif //GHOST_INTERNAL_CONNECTION_FACTORY_HPP
