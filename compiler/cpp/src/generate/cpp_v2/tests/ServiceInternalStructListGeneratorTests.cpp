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

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

#include "../ServiceInternalStructListGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

template <int V>
class ServiceInternalStructGeneratorStub : public ServiceInternalStructGenerator {
public:
  ServiceInternalStructGeneratorStub(OutputGenerator& parent, const t_service* s)
    : ServiceInternalStructGenerator(parent) {
    (void)s;
  }

  void generateDeclaration(const t_function* function) override {
    indented() << function->get_name() << ": declaration" << V << endl;
  }

  void generateDefinition(const t_function* function) override {
    indented() << function->get_name() << ": definition" << V << endl;
  }
};

struct ServiceInternalStructListGeneratorTestsFixture {

  ServiceInternalStructListGeneratorTestsFixture()
    : program("/path"),
      service(&program),
      args(&program),
      fun1(t_base_type::type_void, "fun1", &args),
      fun2(t_base_type::type_void, "fun2", &args) {
    service.set_name("ServiceName");

    service.add_function(&fun1);
    service.add_function(&fun2);
  }

  std::string generateDefinitions() {
    std::ostringstream stream;
    OutputGenerator parent(stream);

    ServiceInternalStructListGenerator gen(parent, &service);
    gen.appendGenerator<ServiceInternalStructGeneratorStub<1>>();
    gen.appendGenerator<ServiceInternalStructGeneratorStub<2>>();
    gen.generateDefinitions();

    return stream.str();
  }

  std::string generateDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);

    ServiceInternalStructListGenerator gen(parent, &service);
    gen.appendGenerator<ServiceInternalStructGeneratorStub<1>>();
    gen.appendGenerator<ServiceInternalStructGeneratorStub<2>>();
    gen.generateDeclarations();

    return stream.str();
  }

  t_program program;
  t_service service;
  t_struct args;
  t_function fun1;
  t_function fun2;
};

BOOST_FIXTURE_TEST_SUITE(ServiceInternalStructListGeneratorTests,
                         ServiceInternalStructListGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(service_generates_declarations_for_all_functions_and_all_generators) {
  BOOST_CHECK_EQUAL(generateDeclarations(),
                    "fun1: declaration1\n"
                    "\n"
                    "fun1: declaration2\n"
                    "\n"
                    "fun2: declaration1\n"
                    "\n"
                    "fun2: declaration2\n"
                    "\n");
}

BOOST_AUTO_TEST_CASE(service_generates_definitions_for_all_functions_and_all_generators) {
  BOOST_CHECK_EQUAL(generateDefinitions(),
                    "fun1: definition1\n"
                    "\n"
                    "fun1: definition2\n"
                    "\n"
                    "fun2: definition1\n"
                    "\n"
                    "fun2: definition2\n"
                    "\n");
}

BOOST_AUTO_TEST_SUITE_END()
