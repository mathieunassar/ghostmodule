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

template<typename Connection>
ConnectionFactoryGenericRule<Connection>::ConnectionFactoryGenericRule(const ghost::ConnectionConfiguration& minimumConfiguration)
	: ConnectionFactoryRule(minimumConfiguration)
{

}

template<typename Connection>
std::shared_ptr<ghost::Connection> ConnectionFactoryGenericRule<Connection>::create(const ghost::ConnectionConfiguration& config) const
{
	return std::make_shared<Connection>(config);
}
