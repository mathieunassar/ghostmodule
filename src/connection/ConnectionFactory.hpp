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

#ifndef GHOST_INTERNAL_CONNECTION_FACTORY_HPP
#define GHOST_INTERNAL_CONNECTION_FACTORY_HPP

#include <memory>
#include <list>

#include <ghost/connection/Server.hpp>
#include <ghost/connection/Client.hpp>
#include <ghost/connection/Publisher.hpp>
#include <ghost/connection/Subscriber.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>

#include <ghost/connection/ConnectionFactory.hpp>


namespace ghost
{
	namespace internal
	{
		class ConnectionFactory : public ghost::ConnectionFactory
		{
		public:
			ConnectionFactory();

			/* Methods to set up the factory */

			void addServerRule(std::shared_ptr<internal::ConnectionFactoryRule> rule) override;
			void addClientRule(std::shared_ptr<internal::ConnectionFactoryRule> rule) override;
			void addPublisherRule(std::shared_ptr<internal::ConnectionFactoryRule> rule) override;
			void addSubscriberRule(std::shared_ptr<internal::ConnectionFactoryRule> rule) override;

			/* Methods to create connections */

			std::shared_ptr<ghost::Server> createServer(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Client> createClient(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Publisher> createPublisher(const ghost::ConnectionConfiguration& config);
			std::shared_ptr<ghost::Subscriber> createSubscriber(const ghost::ConnectionConfiguration& config);

		private:
			std::list<std::shared_ptr<ConnectionFactoryRule>> _serverRules;
			std::list<std::shared_ptr<ConnectionFactoryRule>> _clientRules;
			std::list<std::shared_ptr<ConnectionFactoryRule>> _publisherRules;
			std::list<std::shared_ptr<ConnectionFactoryRule>> _subscriberRules;
		};
	}
}

#endif //GHOST_INTERNAL_CONNECTION_FACTORY_HPP
