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

#include "../ArgsStructBuilder.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ArgsStructBuilderTestsFixture {

  ArgsStructBuilderTestsFixture() : program("/path"), service(&program), builder(&service) {
    service.set_name("ServiceName");
  }

  const t_struct* build(const t_function& function) { return builder.build(&function); }

  t_program program;
  t_service service;
  ArgsStructBuilder builder;
};

BOOST_FIXTURE_TEST_SUITE(ArgsStructBuilderTests, ArgsStructBuilderTestsFixture)

BOOST_AUTO_TEST_CASE(service_and_function_name_reflect_in_struct_name) {
  t_struct args(&program);
  t_function f(t_base_type::type_void, "fun1", &args);

  BOOST_CHECK_EQUAL(build(f)->get_name(), "ServiceName_fun1_args");
}

BOOST_AUTO_TEST_CASE(function_arguments_results_in_struct_fields) {
  t_struct args(&program);
  t_field arg(t_base_type::type_string, "arg1", 10);
  args.append(&arg);
  t_function f(t_base_type::type_void, "fun1", &args);

  auto res = build(f);

  BOOST_REQUIRE_EQUAL(res->get_members().size(), 1);

  auto resMember = res->get_members().front();
  BOOST_CHECK_EQUAL(resMember->get_name(), arg.get_name());
  BOOST_CHECK_EQUAL(resMember->get_type(), arg.get_type());
  BOOST_CHECK_EQUAL(resMember->get_key(), arg.get_key());
}

BOOST_AUTO_TEST_SUITE_END()
