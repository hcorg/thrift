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
#include <parse/t_field.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

#include "../ClientFunctionGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ClientFunctionGeneratorTestsFixture {

  ClientFunctionGeneratorTestsFixture() : program("/path"), service(&program), args(&program) {
    service.set_name("ServiceName");
  }

  std::string generatePublicDeclarations(const t_function& function) {
    std::ostringstream out;
    OutputGenerator parent(out);
    ClientFunctionGenerator gen(parent, &service, &function);
    gen.generatePublicDeclarations();
    return out.str();
  }

  std::string generatePrivateDeclarations(const t_function& function) {
    std::ostringstream out;
    OutputGenerator parent(out);
    ClientFunctionGenerator gen(parent, &service, &function);
    gen.generatePrivateDeclarations();
    return out.str();
  }

  std::string generateDefinitions(const t_function& function) {
    std::ostringstream out;
    OutputGenerator parent(out);
    ClientFunctionGenerator gen(parent, &service, &function);
    gen.generateDefinitions();
    return out.str();
  }

  std::string generatePublicDefinitions(const t_function& function) {
    std::ostringstream out;
    OutputGenerator parent(out);
    ClientFunctionGenerator gen(parent, &service, &function);
    gen.generatePublicDefinitions();
    return out.str();
  }

  t_program program;
  t_service service;
  t_struct args;
};

BOOST_FIXTURE_TEST_SUITE(ClientFunctionGeneratorTests, ClientFunctionGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(void_returning_function_declares_single_public_method) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDeclarations(f), "void functionName() override;\n");
}

BOOST_AUTO_TEST_CASE(void_returning_function_declares_both_private_methods) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePrivateDeclarations(f),
                    "void send_functionName();\n"
                    "void recv_functionName();\n");
}

BOOST_AUTO_TEST_CASE(oneway_function_declares_only_send_method) {
  t_function f(t_base_type::type_void, "functionName", &args, true);
  BOOST_CHECK_EQUAL(generatePrivateDeclarations(f), "void send_functionName();\n");
}

BOOST_AUTO_TEST_CASE(void_returning_function_public_method_call_both_private_ones) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDefinitions(f),
                    "void ServiceNameClient::functionName() {\n"
                    "  send_functionName();\n"
                    "  recv_functionName();\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(oneway_function_calls_only_send) {
  t_function f(t_base_type::type_void, "functionName", &args, true);
  BOOST_CHECK_EQUAL(generatePublicDefinitions(f),
                    "void ServiceNameClient::functionName() {\n"
                    "  send_functionName();\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(void_returning_function_defines_what_it_declares) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generateDefinitions(f),
                    "void ServiceNameClient::functionName() {\n"
                    "  send_functionName();\n"
                    "  recv_functionName();\n"
                    "}\n"
                    "\n"
                    "void ServiceNameClient::send_functionName() {\n"
                    "  ServiceName_functionName_args_refs args = {};\n"
                    "  sendMessage(\"functionName\", ::apache::thrift::protocol::T_CALL, args);\n"
                    "}\n"
                    "\n"
                    "void ServiceNameClient::recv_functionName() {\n"
                    "  ServiceName_functionName_result_ref result;\n"
                    "  recvMessage(\"functionName\", result);\n"
                    "\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(onewayfunction_defines_what_it_declares) {
  t_function f(t_base_type::type_void, "functionName", &args, true);
  BOOST_CHECK_EQUAL(generateDefinitions(f),
                    "void ServiceNameClient::functionName() {\n"
                    "  send_functionName();\n"
                    "}\n"
                    "\n"
                    "void ServiceNameClient::send_functionName() {\n"
                    "  ServiceName_functionName_args_refs args = {};\n"
                    "  sendMessage(\"functionName\", ::apache::thrift::protocol::T_ONEWAY, args);\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(function_args_are_passed_to_send) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  t_field arg2(t_base_type::type_string, "arg2", 2);
  args.append(&arg1);
  args.append(&arg2);
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(
      generatePublicDefinitions(f),
      "void ServiceNameClient::functionName(std::int32_t arg1, const std::string& arg2) {\n"
      "  send_functionName(arg1, arg2);\n"
      "  recv_functionName();\n"
      "}\n");
}

BOOST_AUTO_TEST_CASE(basic_type_returning_function_returns_result_of_recv) {
  t_function f(t_base_type::type_i64, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDefinitions(f),
                    "std::int64_t ServiceNameClient::functionName() {\n"
                    "  send_functionName();\n"
                    "  return recv_functionName();\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_declares_helper_public_method) {
  t_function f(t_base_type::type_string, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDeclarations(f),
                    "std::string functionName() override;\n"
                    "void functionName(std::string& result_);\n");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_with_arguments_declares_helper_public_method) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  args.append(&arg1);
  t_function f(t_base_type::type_string, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDeclarations(f),
                    "std::string functionName(std::int32_t arg1) override;\n"
                    "void functionName(std::string& result_, std::int32_t arg1);\n");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_defines_helper_method) {
  t_function f(t_base_type::type_string, "functionName", &args);
  BOOST_CHECK_EQUAL(generatePublicDefinitions(f),
                    "std::string ServiceNameClient::functionName() {\n"
                    "  std::string result;\n"
                    "  functionName(result);\n"
                    "  return result;\n"
                    "}\n"
                    "\n"
                    "void ServiceNameClient::functionName(std::string& result_) {\n"
                    "  send_functionName();\n"
                    "  recv_functionName(result_);\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_with_arguments_passes_them_to_helper) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  args.append(&arg1);
  t_function f(t_base_type::type_string, "functionName", &args);
  BOOST_CHECK_EQUAL(
      generatePublicDefinitions(f),
      "std::string ServiceNameClient::functionName(std::int32_t arg1) {\n"
      "  std::string result;\n"
      "  functionName(result, arg1);\n"
      "  return result;\n"
      "}\n"
      "\n"
      "void ServiceNameClient::functionName(std::string& result_, std::int32_t arg1) {\n"
      "  send_functionName(arg1);\n"
      "  recv_functionName(result_);\n"
      "}\n");
}

BOOST_AUTO_TEST_SUITE_END()
