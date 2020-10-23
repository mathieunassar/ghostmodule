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

#include <gtest/gtest.h>

#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>

#include "SystemtestExecutorModule.hpp"

class SystemTests : public testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

	std::shared_ptr<SystemtestExecutorModule> _executor;
};

TEST_F(SystemTests, test_system_works)
{
	_executor = std::make_shared<SystemtestExecutorModule>();

	auto builder = ghost::ModuleBuilder::create();
	builder->setInitializeBehavior(
	    std::bind(&SystemtestExecutorModule::initialize, _executor, std::placeholders::_1));
	builder->setRunningBehavior(std::bind(&SystemtestExecutorModule::run, _executor, std::placeholders::_1));
	builder->setDisposeBehavior(std::bind(&SystemtestExecutorModule::dispose, _executor, std::placeholders::_1));
	std::shared_ptr<ghost::Console> console = builder->setConsole();
	builder->setLogger(ghost::GhostLogger::create(console));

	std::shared_ptr<ghost::Module> module = builder->build("systemtest_executor");
	ASSERT_TRUE(module);
	module->start();
}
