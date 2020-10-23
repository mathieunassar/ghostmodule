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

template <typename ConnectionType>
void ConnectionFactory::addServerRule(const ghost::ConnectionConfiguration& config)
{
	addServerRule(std::make_shared<internal::ConnectionFactoryGenericRule<ConnectionType>>(config));
}

template <typename ConnectionType>
void ConnectionFactory::addClientRule(const ghost::ConnectionConfiguration& config)
{
	addClientRule(std::make_shared<internal::ConnectionFactoryGenericRule<ConnectionType>>(config));
}

template <typename ConnectionType>
void ConnectionFactory::addPublisherRule(const ghost::ConnectionConfiguration& config)
{
	addPublisherRule(std::make_shared<internal::ConnectionFactoryGenericRule<ConnectionType>>(config));
}

template <typename ConnectionType>
void ConnectionFactory::addSubscriberRule(const ghost::ConnectionConfiguration& config)
{
	addSubscriberRule(std::make_shared<internal::ConnectionFactoryGenericRule<ConnectionType>>(config));
}
