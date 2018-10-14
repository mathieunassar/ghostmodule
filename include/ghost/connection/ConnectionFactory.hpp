#ifndef GHOST_CONNECTIONFACTORY_HPP
#define GHOST_CONNECTIONFACTORY_HPP

#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	namespace internal
	{
		class ConnectionFactory;
	}

	class ConnectionFactory
	{
	public:
		template<typename Connection>
		void addServerRule(const ghost::ConnectionConfiguration& config);

		template<typename Connection>
		void addClientRule(const ghost::ConnectionConfiguration& config);

		template<typename Connection>
		void addPublisherRule(const ghost::ConnectionConfiguration& config);

		template<typename Connection>
		void addSubscriberRule(const ghost::ConnectionConfiguration& config);

	protected:
		internal::ConnectionFactory* _internal;
	};
}

#endif //GHOST_CONNECTIONFACTORY_HPP
