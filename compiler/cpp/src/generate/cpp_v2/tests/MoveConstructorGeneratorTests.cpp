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

#include "../MoveConstructorGenerator.h"

#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct MoveConstructorGeneratorTestsFixture {

  MoveConstructorGeneratorTestsFixture() : tStruct(nullptr) {}

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    MoveConstructorGenerator gen(parent, &tStruct);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateFieldValue(const t_field& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    MoveConstructorGenerator gen(parent, &tStruct);
    gen.generateFieldValue(&f);
    return stream.str();
  }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    MoveConstructorGenerator gen(parent, &tStruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  t_struct tStruct;
};

BOOST_FIXTURE_TEST_SUITE(MoveConstructorGeneratorTests, MoveConstructorGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_name_is_used_in_declaration) {
  tStruct.set_name("StructName");

  BOOST_CHECK_EQUAL(generateDeclaration(), "StructName(StructName&& other)");
}

BOOST_AUTO_TEST_CASE(empty_struct_shadows_argument_name_in_body) {
  BOOST_CHECK_EQUAL(generateBody(), "(void) other;\n");
}

BOOST_AUTO_TEST_CASE(required_field_generates_init_list_entry_only) {
  t_field f(nullptr, "f", 1);
  f.set_req(t_field::T_REQUIRED);
  tStruct.append(&f);

  BOOST_CHECK(generateBody().empty());
  BOOST_CHECK_EQUAL(generateFieldValue(f), "std::move(other.f)");
}

BOOST_AUTO_TEST_CASE(optional_field_generates_swaps) {
  t_field f(nullptr, "f", 1);
  f.set_req(t_field::T_OPTIONAL);
  tStruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(), "f.swap(other.f);\n");
  BOOST_CHECK(generateFieldValue(f).empty());
}

BOOST_AUTO_TEST_SUITE_END()
