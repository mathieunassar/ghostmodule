#ifndef GHOST_MODULE_HPP
#define GHOST_MODULE_HPP

#include <memory>

#include "internal/Module.hpp"

#include "Console.hpp"
#include "UserManager.hpp"
#include "CommandLineInterpreter.hpp"

namespace Ghost
{
	class Module
	{
	public:
		Module(const std::string& name = "");
		void start();

		static void sleepMillisecond(int ms);

	protected:
		virtual bool init();
		virtual bool run() = 0;
		virtual void dispose();

		/* Console access */
		void initializeConsole();
		bool hasConsole() const;
		std::shared_ptr<Console> getConsole();

		/* Command line interpretor access */
		std::shared_ptr<CommandLineInterpreter> getInterpreter();

		/* User Manager access */
		std::shared_ptr<UserManager> getUserManager();

	private:
		std::shared_ptr<Ghost::internal::Module> _internal;
	};
}

#endif // GHOST_MODULE_HPP
