#include "../include/internal/Prompt.hpp"
#include <regex>

using namespace ghost::internal;

Prompt::Prompt(const std::string& format)
	: _format(format)
	, _user("")
{
	update();
}

std::string Prompt::str() const
{
	return _text;
}

void Prompt::setUser(const std::string& user)
{
	_user = user;
	update();
}

void Prompt::setFormat(const std::string& format)
{
	_format = format;
	update();
}

void Prompt::update()
{
	_text = _format;

	std::regex userRegex("\\$u");
	_text = std::regex_replace(_text, userRegex, _user);

	/*std::regex userRegex("(?<!\\$)\\$u");
	_text = std::regex_replace(_text, userRegex, _user);

	std::regex userRegex2("\\$\\$u");
	_text = std::regex_replace(_text, userRegex2, "$u");*/
}
