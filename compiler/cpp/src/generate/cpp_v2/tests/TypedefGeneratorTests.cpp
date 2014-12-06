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

#include "../TypedefGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_typedef.h>
#include <parse/t_program.h>
#include <parse/t_enum.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct TypedefGeneratorTestsFixture {

  TypedefGeneratorTestsFixture() : program("/path") { program.set_namespace("cpp", "some.name"); }

  std::string generate(const t_typedef& t) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    TypedefGenerator gen(parent, &t);
    gen.generateContents();
    return stream.str();
  }

  std::string header(const t_typedef& t) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    TypedefGenerator gen(parent, &t);
    gen.generateHeader();
    return stream.str();
  }

  t_program program;
};

BOOST_FIXTURE_TEST_SUITE(TypedefGeneratorTests, TypedefGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(simple_typedef_does_not_generate_includes) {
  t_typedef t(&program, t_base_type::type_bool, "TypedefName");
  BOOST_CHECK(header(t).empty());
}

BOOST_AUTO_TEST_CASE(simple_typedef_generates_properly) {
  t_typedef t(&program, t_base_type::type_bool, "TypedefName");
  BOOST_CHECK_EQUAL(generate(t), "using TypedefName = bool;\n");
}

BOOST_AUTO_TEST_CASE(typedef_for_base_type_generates_includes_if_needed) {
  t_typedef t(&program, t_base_type::type_i32, "TypedefName");
  BOOST_CHECK_EQUAL(header(t), "#include <cstdint>\n");
}

BOOST_AUTO_TEST_CASE(typedef_for_user_type_generates_includes) {
  t_enum inner(&program);
  inner.set_name("EnumName");
  t_typedef t(&program, &inner, "TypedefName");

  BOOST_CHECK_EQUAL(header(t), "#include \"EnumName.h\"\n");
}

BOOST_AUTO_TEST_CASE(typedef_for_user_type_generates_properly) {
  t_enum inner(&program);
  inner.set_name("EnumName");
  t_typedef t(&program, &inner, "TypedefName");

  BOOST_CHECK_EQUAL(generate(t), "using TypedefName = EnumName;\n");
}

BOOST_AUTO_TEST_CASE(type_form_other_namespace_generates_properly) {
  t_program other("/other");
  other.set_namespace("cpp", "different.name");
  t_enum inner(&other);
  inner.set_name("EnumName");
  t_typedef t(&program, &inner, "TypedefName");

  BOOST_CHECK_EQUAL(generate(t), "using TypedefName = ::different::name::EnumName;\n");
}

BOOST_AUTO_TEST_SUITE_END()
