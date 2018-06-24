#ifndef GHOST_PROTOBUFCONFIGURATION_HPP
#define GHOST_PROTOBUFCONFIGURATION_HPP

#include <string>
#include <sstream>

#include "../protobuf/Ghost.pb.h"
#include "../protobuf/Ghost.grpc.pb.h"

namespace ghost
{
	namespace internal
	{
		class ProtobufConfiguration
		{
		public:
			ProtobufConfiguration(const std::string& name);

			const std::string& getConfigurationName() const;

			bool hasAttribute(const std::string& name) const;

			template<typename T>
			bool getAttribute(const std::string& name, T& value) const;
			template<typename T>
			bool getAttribute(const std::string& name, T& value, const T& defaultValue) const;

			bool removeAttribute(const std::string& name);

			template<typename T>
			bool addAttribute(const std::string& name, const T& value, bool overwrite = false);

			template<typename T>
			bool updateAttribute(const std::string& name, const T& value);

		private:
			std::string getAttribute(const std::string& name) const;

			internal::protobuf::ConnectionConfiguration _configuration;
		};


		template<typename T>
		bool ProtobufConfiguration::getAttribute(const std::string& name, T& value) const
		{
			if (!hasAttribute(name))
				return false;

			std::istringstream iss(getAttribute(name));
			iss >> value;
			return !iss.fail();
		}

		template<typename T>
		bool ProtobufConfiguration::getAttribute(const std::string& name, T& value, const T& defaultValue) const
		{
			if (!hasAttribute(name))
			{
				value = defaultValue;
				return false;
			}

			std::istringstream iss(getAttribute(name));
			iss >> value;

			if (iss.fail())
				value = defaultValue;

			return !iss.fail();
		}

		template<typename T>
		bool ProtobufConfiguration::addAttribute(const std::string& name, const T& value, bool overwrite)
		{
			if (hasAttribute(name))
			{
				if (!overwrite)
					return false;
				
				return updateAttribute(name, value);
			}

			std::ostringstream oss;
			oss << value;

			internal::protobuf::ConnectionConfigurationField* field = _configuration.add_fields();
			field->set_name(name);
			field->set_value(oss.str());
			return true;
		}

		template<typename T>
		bool ProtobufConfiguration::updateAttribute(const std::string& name, const T& value)
		{
			auto iterator(_configuration.mutable_fields());
			for (auto it = iterator->begin(); it < iterator->end(); ++it)
			{
				if (it->name() == name)
				{
					std::ostringstream oss;
					oss << value;

					it->set_value(oss.str());
					return true;
				}
			}
			return false;
		}
	}
}

#endif //GHOST_PROTOBUFCONFIGURATION_HPP
