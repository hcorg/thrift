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
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>

#include "../SendFunctionGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct SendFunctionGeneratorTestsFixture {

  SendFunctionGeneratorTestsFixture()
    : program("/path"),
      service(&program),
      args(&program),
      function(t_base_type::type_i32, "functionName", &args),
      onewayFunction(t_base_type::type_i32, "functionName", &args, true) {
    service.set_name("ServiceName");
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SendFunctionGenerator gen(parent, &service, &function);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateBody(bool oneway = false) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SendFunctionGenerator gen(parent, &service, oneway ? &onewayFunction : &function);
    gen.generateDefinitionBody();
    return stream.str();
  }

  t_program program;
  t_service service;
  t_struct args;
  t_function function;
  t_function onewayFunction;
};

BOOST_FIXTURE_TEST_SUITE(SendFunctionGeneratorTests, SendFunctionGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(noarg_function_generates_proper_method_name_in_declaration) {
  BOOST_CHECK_EQUAL(generateDeclaration(), "void send_functionName()");
}

BOOST_AUTO_TEST_CASE(function_args_are_passed_to_generated_function) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  t_struct s(&program, "StructArg");
  t_field arg2(&s, "arg2", 2);

  args.append(&arg1);
  args.append(&arg2);

  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "void send_functionName(std::int32_t arg1, const StructArg& arg2)");
}

BOOST_AUTO_TEST_CASE(noarg_functon_generates_basic_body) {
  BOOST_CHECK_EQUAL(generateBody(),
                    "ServiceName_functionName_args_refs args = {};\n"
                    "sendMessage(\"functionName\", ::apache::thrift::protocol::T_CALL, args);\n");
}

BOOST_AUTO_TEST_CASE(oneway_functon_generates_proper_message_type) {
  BOOST_CHECK_EQUAL(generateBody(true),
                    "ServiceName_functionName_args_refs args = {};\n"
                    "sendMessage(\"functionName\", ::apache::thrift::protocol::T_ONEWAY, args);\n");
}

BOOST_AUTO_TEST_CASE(functon_args_are_passed_to_inner_structure) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  t_struct s(&program, "StructArg");
  t_field arg2(&s, "arg2", 2);

  args.append(&arg1);
  args.append(&arg2);

  BOOST_CHECK_EQUAL(generateBody(),
                    "ServiceName_functionName_args_refs args = {arg1, arg2};\n"
                    "sendMessage(\"functionName\", ::apache::thrift::protocol::T_CALL, args);\n");
}

BOOST_AUTO_TEST_SUITE_END()
