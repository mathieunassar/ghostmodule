#ifndef GHOST_LOCALCONSOLE_HPP
#define GHOST_LOCALCONSOLE_HPP


namespace Ghost
{
	/**
	*	Abstract class describing the console window.
	*	Since its methods operate on a lower level, implementations
	*	of this class are operating system specific.
	*/
	class LocalConsole
	{
	public:
		/**
		*	Describes whether the user can see their input on the console
		*	or not.
		*/
		enum ConsoleMode
		{
			INPUT, /// The user can see their input in the console
			OUTPUT /// The user cannot see their input in the console
		};

		/**
		*	Switches the mode of the console window.
		*	Depending on the "mode" parameter, this method enables or diables
		*	the visualisation of text in the console window (@see ConsoleMode)
		*	@param mode		new mode to set
		*	@return true on success, false otherwise
		*/
		virtual bool setConsoleMode(ConsoleMode mode) = 0;
	};
}

#endif //GHOST_LOCALCONSOLE_HPP
