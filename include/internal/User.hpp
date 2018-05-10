#ifndef GHOST_INTERNAL_USER_HPP
#define GHOST_INTERNAL_USER_HPP

#include <string>
#include "../User.hpp"

namespace Ghost
{
	namespace internal
	{
		class User : public ::Ghost::User
		{
		public:
			User(const std::string& name, const std::string& password);

			const std::string& getName() const override;

			bool contains(const PermissionEntity& other) const override;

			bool isPasswordCorrect(const std::string& password) const;

		private:
			std::string _name;
			std::string _password;
		};
	}
	
}

#endif // GHOST_INTERNAL_USER_HPP
