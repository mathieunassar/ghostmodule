#ifndef GHOST_INTERNAL_PROMPT_HPP
#define GHOST_INTERNAL_PROMPT_HPP

#include <string>

namespace ghost
{
	namespace internal
	{
		class Prompt
		{
		public:
			Prompt(const std::string& format);

			std::string str() const;

			void setUser(const std::string& user);
			void setFormat(const std::string& format);

		private:
			void update();

			std::string _text;
			std::string _format;
			std::string _user;
		};
	}
}

#endif // GHOST_INTERNAL_PROMPT_HPP
