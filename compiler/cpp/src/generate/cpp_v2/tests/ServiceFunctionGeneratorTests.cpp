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

#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "../ServiceFunctionGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ServiceFunctionGeneratorTestsFixture {

  ServiceFunctionGeneratorTestsFixture() : args(nullptr) {}

  std::string generateDeclaration(const t_function& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ServiceFunctionGenerator gen(parent, "ServiceName", &f);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateDefinition(const t_function& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ServiceFunctionGenerator gen(parent, "ServiceName", &f);
    gen.generateDefinition();
    return stream.str();
  }

  t_struct args;
};

BOOST_FIXTURE_TEST_SUITE(ServiceFunctionGeneratorTests, ServiceFunctionGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(return_type_and_name_used_in_declaration) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generateDeclaration(f), "void functionName()");
}

BOOST_AUTO_TEST_CASE(class_name_is_used_in_definition) {
  t_function f(t_base_type::type_string, "fun", &args);
  BOOST_CHECK_EQUAL(generateDefinition(f),
                    "std::string ServiceName::fun() {\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(args_are_used_in_declaration) {
  t_field arg1(t_base_type::type_string, "arg1", 1);
  t_field arg2(t_base_type::type_string, "arg2", 2);
  args.append(&arg1);
  args.append(&arg2);

  t_function f(t_base_type::type_void, "functionWithArgs", &args);

  BOOST_CHECK_EQUAL(generateDeclaration(f),
                    "void functionWithArgs(const std::string& arg1, const std::string& arg2)");
}

BOOST_AUTO_TEST_SUITE_END()
