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

#include "../ProcessorRegisterDispatchesGenerator.h"

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ProcessorRegisterDispatchesGeneratorTestsFixture {

  ProcessorRegisterDispatchesGeneratorTestsFixture() : program("/path"), service(&program) {}

  std::string generate() {
    std::ostringstream out;
    OutputGenerator parent(out);
    ProcessorRegisterDispatchesGenerator gen(parent, &service);
    gen.generate();
    return out.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(ProcessorRegisterDispatchesGeneratorTests,
                         ProcessorRegisterDispatchesGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(empty_service_generates_empty_registry) {
  BOOST_CHECK_EQUAL(generate(),
                    "template <typename DispatcherType>\n"
                    "void registerDispatches(DispatcherType& dispatcher) {\n"
                    "  (void) dispatcher;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(extended_service_calls_parent_register) {
  // TODO
}

BOOST_AUTO_TEST_CASE(service_methods_are_registered) {
  // TODO
}

BOOST_AUTO_TEST_SUITE_END()
