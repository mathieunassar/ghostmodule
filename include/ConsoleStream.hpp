#ifndef GHOST_CONSOLESTREAM_HPP
#define GHOST_CONSOLESTREAM_HPP

#include <streambuf>
#include <iostream>
#include <sstream>
#include <functional>

namespace Ghost
{
	/**
	 *	This class inherits std::basic_streambug to forward the stream to a given
	 *	callback function.
	 */
	template <class Type = char, class Traits = std::char_traits<Type>>
	class ConsoleStream : public std::basic_streambuf<Type, Traits>
	{
	public:
		ConsoleStream(std::ostream &stream, std::function<void(const Type*, std::streamsize count)> consoleCallback)
			: _stream(stream), _consoleCallback(consoleCallback)
		{
			// redirect stream
			_buf = _stream.rdbuf(this);
		};

		~ConsoleStream()
		{
			// restore stream
			_stream.rdbuf(_buf);
		}

		// override xsputn and make it forward data to the callback function
		std::streamsize xsputn(const Type *ptr, std::streamsize count)
		{
			_consoleCallback(ptr, count);
			return count;
		}

		// override overflow and make it forward data to the callback function
		typename Traits::int_type overflow(typename Traits::int_type v)
		{
			Type ch = Traits::to_char_type(v);
			_consoleCallback(&ch, 1);
			return Traits::not_eof(v);
		}

	private:
		std::function<void(const Type*, std::streamsize count)> _consoleCallback;
		std::basic_ostream<Type, Traits>& _stream;
		std::streambuf* _buf;
	};
}

#endif // GHOST_CONSOLESTREAM_HPP
