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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <ghost/module/StdoutLogger.hpp>
#include <thread>

#include "../src/module/Module.hpp"

class ModuleExtensionMock : public ghost::ModuleExtension
{
public:
	static const std::string getExtensionName();

	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, void());
	MOCK_CONST_METHOD0(getName, std::string());
};

class ModuleExtensionBuilderMock : public ghost::ModuleExtensionBuilder
{
public:
	MOCK_METHOD0(build, std::shared_ptr<ghost::ModuleExtension>());
};

class ModuleTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_builder = ghost::ModuleBuilder::create();
		_builderMock = std::make_shared<ModuleExtensionBuilderMock>();
		_componentMock = std::make_shared<ModuleExtensionMock>();

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

	std::unique_ptr<ghost::ModuleBuilder> _builder;
	std::shared_ptr<ModuleExtensionBuilderMock> _builderMock;
	std::shared_ptr<ModuleExtensionMock> _componentMock;
	bool _initIsCalled;
	bool _initReturn;
	bool _runIsCalled;
	bool _runWaits;
	bool _runReturn;
	bool _disposeIsCalled;

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

	static const std::string TEST_MODULE_NAME;
};

const std::string ModuleTest::TEST_MODULE_NAME = "TestModule";

const std::string ModuleExtensionMock::getExtensionName()
{
	return ModuleTest::TEST_MODULE_NAME;
}

TEST_F(ModuleTest, Test_ModuleBuilder_simpleModule)
{
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);
	ASSERT_TRUE(module->getModuleName() == TEST_MODULE_NAME);
	ASSERT_TRUE(module->getInterpreter());
	ASSERT_TRUE(module->getUserManager());
}

TEST_F(ModuleTest, Test_ModuleBuilder_intializeBehavior_isCalled)
{
	_builder->setInitializeBehavior(std::bind(&ModuleTest::init, this, std::placeholders::_1));
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_initIsCalled);
	module->start();
	ASSERT_TRUE(_initIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_runningBehavior_isCalled)
{
	_builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_runIsCalled);
	module->start();
	ASSERT_TRUE(_runIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_disposeBehavior_isCalled)
{
	_builder->setDisposeBehavior(std::bind(&ModuleTest::dispose, this, std::placeholders::_1));
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_FALSE(_disposeIsCalled);
	module->start();
	ASSERT_TRUE(_disposeIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_logger_isAccessible)
{
	auto logger = ghost::StdoutLogger::create();
	_builder->setLogger(logger);
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_TRUE(module->getLogger());
	ASSERT_TRUE(module->getLogger().get() == logger.get());
}

TEST_F(ModuleTest, Test_ModuleBuilder_programOptions_isAccessible)
{
	int argc = 1;
	char* argv[1] = {(char*)TEST_MODULE_NAME.c_str()};
	_builder->setProgramOptions(argc, argv);
	auto module = _builder->build(TEST_MODULE_NAME);
	ASSERT_TRUE(module);

	ASSERT_TRUE(module->getProgramOptions().getCommandName() == TEST_MODULE_NAME);
}

TEST_F(ModuleTest, Test_ModuleBuilder_component_isAccessible)
{
	// expected two calls: one from the "getComponent" call, one from this test
	EXPECT_CALL(*_componentMock, getName).Times(2).WillRepeatedly(testing::Return(TEST_MODULE_NAME));
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(_componentMock));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_TRUE(module);

	auto retrievedComponent = module->getExtension<ModuleExtensionMock>();
	ASSERT_TRUE(retrievedComponent);
	ASSERT_TRUE(retrievedComponent->getName() == TEST_MODULE_NAME);
}

TEST_F(ModuleTest, Test_ModuleBuilder_componentBuilderFails)
{
	// build fails, returns nullptr
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(nullptr));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_FALSE(module);
}

TEST_F(ModuleTest, Test_ModuleBuilder_componentIsStarted_When_moduleIsStarted)
{
	EXPECT_CALL(*_componentMock, start).Times(1).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*_componentMock, stop).Times(1);
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(_componentMock));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_TRUE(module);
	module->start();
}

TEST_F(ModuleTest, Test_ModuleBuilder_moduleRuns_When_componentStartSucceeds)
{
	EXPECT_CALL(*_componentMock, start).Times(1).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*_componentMock, stop).Times(1);
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(_componentMock));
	_builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_TRUE(module);
	module->start();
	ASSERT_TRUE(_runIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_moduleFails_When_componentStartFails)
{
	EXPECT_CALL(*_componentMock, start).Times(1).WillRepeatedly(testing::Return(false));
	EXPECT_CALL(*_componentMock, stop).Times(1);
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(_componentMock));
	_builder->setRunningBehavior(std::bind(&ModuleTest::run, this, std::placeholders::_1));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_TRUE(module);
	module->start();
	ASSERT_FALSE(_runIsCalled);
}

TEST_F(ModuleTest, Test_ModuleBuilder_componentIsNotStopped_When_moduleIsAlreadyStopped)
{
	EXPECT_CALL(*_componentMock, stop).Times(0);
	EXPECT_CALL(*_builderMock, build()).Times(1).WillOnce(testing::Return(_componentMock));

	_builder->addExtensionBuilder(_builderMock);
	auto module = _builder->build();
	ASSERT_TRUE(module);
	module->stop();
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

TEST_F(ModuleTest, Test_Session_generatesUniqueUuid_When_createIsCalled)
{
	auto session = ghost::Session::create();
	auto session2 = ghost::Session::create();
	ASSERT_NE(session->getUUID(), session2->getUUID());
}