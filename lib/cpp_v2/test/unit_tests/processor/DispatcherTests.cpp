/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <boost/test/auto_unit_test.hpp>

#include <thrift/processor/Dispatcher.h>

using namespace apache::thrift::processor;

struct DispatcherParentDummy {

  void processUnknownFunction(const std::string&, ProcessArgs) { unknownFunctionRequested = true; }

  void method1(ProcessArgs) { methodWasCalled = true; }

  void method2(ProcessArgs) { method2WasCalled = true; }

  bool unknownFunctionRequested = false;
  bool methodWasCalled = false;
  bool method2WasCalled = false;
};

using TestedDispatcher = apache::thrift::processor::Dispatcher<DispatcherParentDummy>;

struct DispatcherTestsFixture {

  DispatcherTestsFixture() : dispatcher(&dummy) {}

  void dispatch(const std::string& name) {
    ProcessArgs args;
    dispatcher.dispatchByName(name, args);
  }

  DispatcherParentDummy dummy;
  TestedDispatcher dispatcher;
};

BOOST_FIXTURE_TEST_SUITE(DispatcherTests, DispatcherTestsFixture)

BOOST_AUTO_TEST_CASE(empty_dispatcher_calls_processUnknownFunction) {
  dispatch("any_name");
  BOOST_CHECK(dummy.unknownFunctionRequested);
}

BOOST_AUTO_TEST_CASE(dispatcher_calls_registered_method) {
  dispatcher.registerDispatch("method1", &DispatcherParentDummy::method1);
  dispatch("method1");
  BOOST_CHECK(dummy.methodWasCalled);
}

BOOST_AUTO_TEST_CASE(dispatcher_calls_processUnknownFunction_on_bad_name) {
  dispatcher.registerDispatch("method1", &DispatcherParentDummy::method1);
  dispatch("wrongName");
  BOOST_CHECK(!dummy.methodWasCalled);
  BOOST_CHECK(dummy.unknownFunctionRequested);
}

BOOST_AUTO_TEST_CASE(dispatcher_calls_proper_method_by_name) {
  dispatcher.registerDispatch("method1", &DispatcherParentDummy::method1);
  dispatcher.registerDispatch("method2", &DispatcherParentDummy::method2);
  dispatch("method2");
  BOOST_CHECK(!dummy.methodWasCalled);
  BOOST_CHECK(dummy.method2WasCalled);
}

BOOST_AUTO_TEST_SUITE_END()
