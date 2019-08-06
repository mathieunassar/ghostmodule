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
void ConnectionFactory::addServerRule(const ghost::ConnectionConfiguration& config)
{
	_serverRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addClientRule(const ghost::ConnectionConfiguration& config)
{
	_clientRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addPublisherRule(const ghost::ConnectionConfiguration& config)
{
	_publisherRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}

template<typename Connection>
void ConnectionFactory::addSubscriberRule(const ghost::ConnectionConfiguration& config)
{
	_subscriberRules.emplace_back(new ConnectionFactoryGenericRule<Connection>(config));
}
