/*
 * Copyright 2019 Mathieu Nassar
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

#ifndef GHOST_INTERNAL_WRITER_HPP
#define GHOST_INTERNAL_WRITER_HPP

#include <ghost/connection/Writer.hpp>

#include "WriterSink.hpp"

namespace ghost
{
namespace internal
{
/**
 *	The internal implementation of the ghost::Writer is a specialized
 *	type handling google::protobuf::Any messages in order to push them
 *	to the WriterSink object.
 */
class Writer : public ghost::Writer<google::protobuf::Any>
{
public:
	Writer(const std::shared_ptr<ghost::WriterSink>& sink, bool blocking);

	bool write(const google::protobuf::Any& message) override;

private:
	std::shared_ptr<WriterSink> _writerSink;
	bool _blocking;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_INTERNAL_WRITER_HPP
