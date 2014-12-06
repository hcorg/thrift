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

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "../SingletonHandlerFactoryGenerator.h"

#include <parse/t_service.h>
#include <parse/t_program.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct SingletonHandlerFactoryGeneratorTestsFixture {

  SingletonHandlerFactoryGeneratorTestsFixture() : program("/path"), service(&program) {
    service.set_name("ServiceName");
  }

  std::string generateHeader() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SingletonHandlerFactoryGenerator gen(parent, &service);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateContents() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SingletonHandlerFactoryGenerator gen(parent, &service);
    gen.generateContents();
    return stream.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(SingletonHandlerFactoryGeneratorTests,
                         SingletonHandlerFactoryGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(basic_service_includes_service_handler_factory) {
  BOOST_CHECK_EQUAL(generateHeader(),
                    "#include <memory>\n"
                    "\n"
                    "#include \"ServiceNameHandlerFactory.h\"\n");
}

BOOST_AUTO_TEST_CASE(basic_service_results_in_complete_class) {
  BOOST_CHECK_EQUAL(
      generateContents(),
      "class ServiceNameSingletonHandlerFactory : public virtual ServiceNameHandlerFactory {\n"
      "public:\n"
      "  ServiceNameSingletonHandlerFactory(const std::shared_ptr<Handler>& handler)\n"
      "    : handler_(handler) {}\n"
      "\n"
      "  Handler* getHandler(const ::apache::thrift::TConnectionInfo&) override {\n"
      "    return handler_.get();\n"
      "  }\n"
      "\n"
      "  void releaseHandler(Handler*) override {}\n"
      "\n"
      "private:\n"
      "  std::shared_ptr<Handler> handler_;\n"
      "};\n");
}

BOOST_AUTO_TEST_SUITE_END()
