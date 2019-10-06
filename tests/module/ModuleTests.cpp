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

#include <gtest/gtest.h>

#include <chrono>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <ghost/module/StdoutLogger.hpp>
#include <thread>

#include "../src/module/Module.hpp"

class ModuleTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_initReturn = true;
		_initIsCalled = false;
		_runReturn = false;
		_runWaits = false;
		_runIsCalled = false;
		_disposeIsCalled = false;
	}

	void TearDown() override
	{
	}

	bool _initIsCalled;
	bool _initReturn;
	bool _runIsCalled;
	bool _runWaits;
	bool _runReturn;
	bool _disposeIsCalled;

	static const std::string TEST_MODULE_NAME;

public:
	bool init(const ghost::Module& module)
	{
		_initIsCalled = true;
		return _initReturn;
	}

	bool run(const ghost::Module& module)
	{
		_runIsCalled = true;
		if (_runWaits) std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return _runReturn;
	}

	void dispose(const ghost::Module& module)
	{
		_disposeIsCalled = true;
	}
};

const std::string ModuleTest::TEST_MODULE_NAME = "TestModule";

TEST_F(ModuleTest, Test_ModuleBuilder_simpleModule)
{
	auto builder = ghost::ModuleBuilder::create();
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);
	ASSERT_TRUE(module->getModuleName() == TEST_MODULE_NAME);
	ASSERT_TRUE(module->getInterpreter());
	ASSERT_TRUE(module->getUserManager());
}

TEST_F(ModuleTest, Test_ModuleBuilder_intializeBehaviorIsCalled)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setInitializeBehavior(std::bind(&ModuleTest::init, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_initIsCalled);
	module->start();
	ASSERT_TRUE(_initIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_runningBehaviorIsCalled)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_runIsCalled);
	module->start();
	ASSERT_TRUE(_runIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_disposeBehaviorIsCalled)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setDisposeBehavior(std::bind(&ModuleTest::dispose, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_disposeIsCalled);
	module->start();
	ASSERT_TRUE(_disposeIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_logger)
{
	auto builder = ghost::ModuleBuilder::create();
	auto logger = ghost::StdoutLogger::create();
	builder->setLogger(logger);
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_TRUE(module->getLogger());
	ASSERT_TRUE(module->getLogger().get() == logger.get());
}

TEST_F(ModuleTest, Test_ModuleBuilder_programOptions)
{
	auto builder = ghost::ModuleBuilder::create();
	int argc = 1;
	char* argv[1] = {(char*)TEST_MODULE_NAME.c_str()};
	builder->setProgramOptions(argc, argv);
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_TRUE(module->getProgramOptions().getCommandName() == TEST_MODULE_NAME);
}

TEST_F(ModuleTest, Test_Module_intializeBehaviorFails)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setInitializeBehavior(std::bind(&ModuleTest::init, this, std::placeholders::_1));
	builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));
	builder->setDisposeBehavior(std::bind(&ModuleTest::dispose, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	_initReturn = false;

	ASSERT_FALSE(_initIsCalled);
	module->start();
	ASSERT_TRUE(_initIsCalled);
	ASSERT_FALSE(_runIsCalled);
	ASSERT_TRUE(_disposeIsCalled);
}

TEST_F(ModuleTest, Test_Module_startsTwice)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setInitializeBehavior(std::bind(&ModuleTest::init, this, std::placeholders::_1));
	builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));
	builder->setDisposeBehavior(std::bind(&ModuleTest::dispose, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	_runWaits = true;

	std::thread t([&] { module->start(); });
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait that the thread reaches init
	ASSERT_TRUE(_initIsCalled);
	_initIsCalled = false;

	std::thread t2([&] { module->start(); }); // call it a second time, should not go through init again

	t.join();
	t2.join();
	ASSERT_FALSE(_initIsCalled);
}

TEST_F(ModuleTest, Test_Module_stops)
{
	auto builder = ghost::ModuleBuilder::create();
	builder->setInitializeBehavior(std::bind(&ModuleTest::init, this, std::placeholders::_1));
	builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));
	builder->setDisposeBehavior(std::bind(&ModuleTest::dispose, this, std::placeholders::_1));
	auto module = builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	_runWaits = true;
	_runReturn = true;
	std::thread t([&] { module->start(); });
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // wait that the thread reaches init
	ASSERT_TRUE(_initIsCalled);

	module->stop();
	t.join();
}
