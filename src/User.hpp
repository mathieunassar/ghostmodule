#ifndef GHOST_INTERNAL_USER_HPP
#define GHOST_INTERNAL_USER_HPP

#include <string>
#include <ghost/module/User.hpp>

namespace ghost
{
	namespace internal
	{
		class User : public ghost::User
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
