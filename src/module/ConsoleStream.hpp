/*
 * Copyright 2020 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GHOST_INTERNAL_CONSOLESTREAM_HPP
#define GHOST_INTERNAL_CONSOLESTREAM_HPP

#include <functional>
#include <iostream>
#include <sstream>
#include <streambuf>

namespace ghost
{
namespace internal
{
/**
 *	This class inherits std::basic_streambug to forward the stream to a given
 *	callback function.
 */
template <class Type = char, class Traits = std::char_traits<Type>>
class ConsoleStream : public std::basic_streambuf<Type, Traits>
{
public:
	ConsoleStream(std::ostream& stream, std::function<void(const Type*, std::streamsize count)> consoleCallback)
	    : _consoleCallback(consoleCallback), _stream(stream)
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
	std::streamsize xsputn(const Type* ptr, std::streamsize count)
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
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONSOLESTREAM_HPP
