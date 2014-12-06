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

#include <boost/test/unit_test.hpp>

#include "../ResultStructBuilder.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ResultStructBuilderTestsFixture {

  ResultStructBuilderTestsFixture() : program("/path"), service(&program), builder(&service) {
    service.set_name("ServiceName");
  }

  const t_struct* build(const t_function& function) { return builder.build(&function); }

  t_program program;
  t_service service;
  ResultStructBuilder builder;
};

BOOST_FIXTURE_TEST_SUITE(ResultStructBuilderTests, ResultStructBuilderTestsFixture)

BOOST_AUTO_TEST_CASE(service_and_function_name_reflect_in_struct_name) {
  t_struct args(&program);
  t_function f(t_base_type::type_void, "fun1", &args);

  BOOST_CHECK_EQUAL(build(f)->get_name(), "ServiceName_fun1_result");
}

BOOST_AUTO_TEST_CASE(functions_with_void_return_generate_empty_structs) {
  t_struct args1(&program);
  t_function f1(t_base_type::type_void, "fun1", &args1);

  BOOST_CHECK(build(f1)->get_members().empty());
}

BOOST_AUTO_TEST_CASE(functions_with_non_void_return_generate_structs_with_optional_success_field) {
  t_struct args1(&program);
  t_function f1(t_base_type::type_i32, "fun1", &args1);

  BOOST_REQUIRE_EQUAL(build(f1)->get_members().size(), 1u);

  auto res = build(f1)->get_members().front();

  BOOST_CHECK_EQUAL(res->get_name(), "success");
  BOOST_CHECK_EQUAL(res->get_type(), t_base_type::type_i32);
  BOOST_CHECK_EQUAL(res->get_key(), 0);
  BOOST_CHECK_EQUAL(res->get_req(), t_field::T_OPTIONAL);
}

BOOST_AUTO_TEST_CASE(functions_with_exceptions_add_them_as_optionals_to_generated_structs) {
  t_struct args1(&program, "fun1_args");
  t_struct exceptions(&program, "fun1_exceptions");
  t_field e(t_base_type::type_i32, "e1", 10);
  exceptions.append(&e);
  t_function f1(t_base_type::type_void, "fun1", &args1, &exceptions);

  BOOST_REQUIRE_EQUAL(build(f1)->get_members().size(), 1u);

  auto res = build(f1)->get_members().front();

  BOOST_CHECK_EQUAL(res->get_name(), e.get_name());
  BOOST_CHECK_EQUAL(res->get_type(), e.get_type());
  BOOST_CHECK_EQUAL(res->get_key(), e.get_key());
  BOOST_CHECK_EQUAL(res->get_req(), t_field::T_OPTIONAL);
}

BOOST_AUTO_TEST_SUITE_END()
