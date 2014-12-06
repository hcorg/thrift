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

#include <boost/test/unit_test.hpp>

#include "../TypeGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_type.h>
#include <parse/t_enum.h>
#include <parse/t_struct.h>
#include <parse/t_service.h>
#include <parse/t_typedef.h>
#include <parse/t_program.h>
#include <parse/t_list.h>
#include <parse/t_set.h>
#include <parse/t_map.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct TypeGeneratorTestsFixture {

  std::string generate(const t_type* type) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    TypeGenerator gen(parent);
    gen.setCurrentNamespace("some.ns");
    gen.generate(type);
    return stream.str();
  }
};

BOOST_FIXTURE_TEST_SUITE(TypeGeneratorTests, TypeGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(null_type_throws) {
  BOOST_CHECK_THROW(generate(nullptr), std::string);
}

BOOST_AUTO_TEST_CASE(byte_type_generates_int8_t) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_byte), "std::int8_t");
}

BOOST_AUTO_TEST_CASE(i16_type_generates_int16_t) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_i16), "std::int16_t");
}

BOOST_AUTO_TEST_CASE(i16_type_generates_int32_t) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_i32), "std::int32_t");
}

BOOST_AUTO_TEST_CASE(i16_type_generates_int64_t) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_i64), "std::int64_t");
}

BOOST_AUTO_TEST_CASE(bool_type_generates_bool) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_bool), "bool");
}

BOOST_AUTO_TEST_CASE(double_type_generates_double) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_double), "double");
}

BOOST_AUTO_TEST_CASE(void_type_generates_void) {
  BOOST_CHECK_EQUAL(generate(t_base_type::type_void), "void");
}

BOOST_AUTO_TEST_CASE(enum_type_generates_enum_name) {
  t_enum t(nullptr);
  t.set_name("EnumName");

  BOOST_CHECK_EQUAL(generate(&t), "EnumName");
}

BOOST_AUTO_TEST_CASE(struct_type_generates_struct_name) {
  t_struct t(nullptr, "StructName");

  BOOST_CHECK_EQUAL(generate(&t), "StructName");
}

BOOST_AUTO_TEST_CASE(exception_type_generates_exception_name) {
  t_struct t(nullptr, "XName");
  t.set_xception(true);

  BOOST_CHECK_EQUAL(generate(&t), "XName");
}

BOOST_AUTO_TEST_CASE(typedef_type_generates_typedef_name) {
  t_struct s(nullptr, "XName");
  t_typedef t(nullptr, &s, "Def");

  BOOST_CHECK_EQUAL(generate(&t), "Def");
}

BOOST_AUTO_TEST_CASE(service_type_generates_service_name) {
  t_service t(nullptr);
  t.set_name("ServiceName");

  BOOST_CHECK_EQUAL(generate(&t), "ServiceName");
}

BOOST_AUTO_TEST_CASE(type_from_different_namespace_appends_namespace_prefix) {
  t_program program("/path");
  program.set_namespace("cpp", "different.namespace");
  t_struct t(&program, "StructName");

  BOOST_CHECK_EQUAL(generate(&t), "::different::namespace::StructName");
}

BOOST_AUTO_TEST_CASE(list_type_generates_vector) {
  t_list l(t_base_type::type_byte);
  BOOST_CHECK_EQUAL(generate(&l), "std::vector<std::int8_t>");
}

BOOST_AUTO_TEST_CASE(list_type_generates_subsituted_type) {
  t_list l(t_base_type::type_byte);
  l.set_cpp_name("QList");

  BOOST_CHECK_EQUAL(generate(&l), "QList");
}

BOOST_AUTO_TEST_CASE(set_type_generates_unordered_set) {
  t_set c(t_base_type::type_byte);
  BOOST_CHECK_EQUAL(generate(&c), "std::unordered_set<std::int8_t>");
}

BOOST_AUTO_TEST_CASE(set_type_generates_subsituted_type) {
  t_set c(t_base_type::type_byte);
  c.set_cpp_name("QSet");

  BOOST_CHECK_EQUAL(generate(&c), "QSet");
}

BOOST_AUTO_TEST_CASE(map_type_generates_unordered_set) {
  t_map c(t_base_type::type_string, t_base_type::type_i32);
  BOOST_CHECK_EQUAL(generate(&c), "std::unordered_map<std::string, std::int32_t>");
}

BOOST_AUTO_TEST_CASE(map_type_generates_subsituted_type) {
  t_map c(t_base_type::type_string, t_base_type::type_i32);
  c.set_cpp_name("QMap");

  BOOST_CHECK_EQUAL(generate(&c), "QMap");
}

BOOST_AUTO_TEST_SUITE_END()
