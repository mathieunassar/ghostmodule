#include "../include/internal/User.hpp"

using namespace Ghost::internal;

User::User(const std::string& name, const std::string& password)
	: _name(name)
	, _password(password)
{

}

const std::string& User::getName() const
{
	return _name;
}

bool User::contains(const PermissionEntity& other) const
{
	const User* cast = dynamic_cast<const User*>(&other);
	if (cast != nullptr)
	{
		return cast->getName() == _name;
	}
	return false;
}

bool User::isPasswordCorrect(const std::string& password) const
{
	return _password == password;
}
