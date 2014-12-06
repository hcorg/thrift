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

#include "../RecvFunctionGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct RecvFunctionGeneratorTestsFixture {

  RecvFunctionGeneratorTestsFixture() : program("/path"), service(&program), args(&program) {
    service.set_name("ServiceName");
  }

  std::string generateDeclaration(t_function& function) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    RecvFunctionGenerator gen(parent, &service, &function);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateBody(t_function& function) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    RecvFunctionGenerator gen(parent, &service, &function);
    gen.generateDefinitionBody();
    return stream.str();
  }

  t_program program;
  t_service service;
  t_struct args;
};

BOOST_FIXTURE_TEST_SUITE(RecvFunctionGeneratorTests, RecvFunctionGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(void_returning_function_generates_proper_method_name_in_declaration) {
  t_function f(t_base_type::type_void, "functionName", &args);
  BOOST_CHECK_EQUAL(generateDeclaration(f), "void recv_functionName()");
}

BOOST_AUTO_TEST_CASE(basic_type_returning_function_generates_recv_via_normal_return) {
  t_function f(t_base_type::type_i64, "functionName", &args);
  BOOST_CHECK_EQUAL(generateDeclaration(f), "std::int64_t recv_functionName()");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_generates_recv_via_reference) {
  t_function f(t_base_type::type_string, "functionName", &args);
  BOOST_CHECK_EQUAL(generateDeclaration(f), "void recv_functionName(std::string& result_)");
}

BOOST_AUTO_TEST_CASE(void_returning_function_generates_simple_recv) {
  t_function f(t_base_type::type_void, "functionName", &args);

  BOOST_CHECK_EQUAL(generateBody(f),
                    "ServiceName_functionName_result_ref result;\n"
                    "recvMessage(\"functionName\", result);\n"
                    "\n");
}

BOOST_AUTO_TEST_CASE(basic_type_returning_function_generates_returning_recv) {
  t_function f(t_base_type::type_i32, "functionName", &args);

  BOOST_CHECK_EQUAL(generateBody(f),
                    "ServiceName_functionName_result_ref result;\n"
                    "recvMessage(\"functionName\", result);\n"
                    "\n"
                    "if (result.success) {\n"
                    "  return *result.success;\n"
                    "}\n"
                    "throw "
                    "::apache::thrift::TApplicationException(::apache::thrift::"
                    "TApplicationException::MISSING_RESULT, \"ServiceName::functionName failed: "
                    "unknown result\");\n");
}

BOOST_AUTO_TEST_CASE(complex_type_returning_function_generates_recv_using_refs) {
  t_function f(t_base_type::type_string, "functionName", &args);

  BOOST_CHECK_EQUAL(generateBody(f),
                    "ServiceName_functionName_result_ref result;\n"
                    "result.success = result_;\n"
                    "recvMessage(\"functionName\", result);\n"
                    "\n"
                    "if (result.success) {\n"
                    "  return;\n"
                    "}\n"
                    "throw "
                    "::apache::thrift::TApplicationException(::apache::thrift::"
                    "TApplicationException::MISSING_RESULT, \"ServiceName::functionName failed: "
                    "unknown result\");\n");
}

BOOST_AUTO_TEST_CASE(throwing_function_generates_throwing_recv) {
  t_struct excepts(&program);
  t_field e(t_base_type::type_string, "e1", 1);
  excepts.append(&e);
  t_function f(t_base_type::type_void, "functionName", &args, &excepts);

  BOOST_CHECK_EQUAL(generateBody(f),
                    "ServiceName_functionName_result_ref result;\n"
                    "recvMessage(\"functionName\", result);\n"
                    "\n"
                    "if (result.e1) {\n"
                    "  throw *result.e1;\n"
                    "}\n"
                    "throw "
                    "::apache::thrift::TApplicationException(::apache::thrift::"
                    "TApplicationException::MISSING_RESULT, \"ServiceName::functionName failed: "
                    "unknown result\");\n");
}

BOOST_AUTO_TEST_SUITE_END()
