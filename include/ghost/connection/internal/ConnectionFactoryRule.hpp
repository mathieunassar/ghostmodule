#ifndef GHOST_INTERNAL_CONNECTIONFACTORYRULE_HPP
#define GHOST_INTERNAL_CONNECTIONFACTORYRULE_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	namespace internal
	{
		/* base class */
		class ConnectionFactoryRule
		{
		public:
			ConnectionFactoryRule(const ghost::ConnectionConfiguration& minimumConfiguration);
			virtual ~ConnectionFactoryRule() = 0;

			/// returns true if the candidate configuration fulfills the minimum configuration requirements
			/// Note: empty values in the minimum configuration mean that any value is allowed for this parameter
			bool matches(const ghost::ConnectionConfiguration& candidate) const;
			/// creates an instance of the corresponding connection, with the given configuration in parameters.
			virtual std::shared_ptr<ghost::Connection> create(const ghost::ConnectionConfiguration& config) const = 0;

		protected:
			ghost::ConnectionConfiguration _minimumConfiguration;
		};

		/* Generic class */
		template<typename Connection>
		class ConnectionFactoryGenericRule : public ConnectionFactoryRule
		{
		public:
			ConnectionFactoryGenericRule(const ghost::ConnectionConfiguration& minimumConfiguration);

			std::shared_ptr<ghost::Connection> create(const ghost::ConnectionConfiguration& config) const override;
		};

		inline ConnectionFactoryRule::~ConnectionFactoryRule() {}

		#include "ConnectionFactoryRule.impl.hpp"
	}
}

#endif //GHOST_INTERNAL_CONNECTIONFACTORYRULE_HPP
