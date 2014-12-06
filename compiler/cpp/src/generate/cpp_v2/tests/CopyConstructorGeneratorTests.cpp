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

#include "../CopyConstructorGenerator.h"

#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct CopyConstructorGeneratorTestsFixture {

  CopyConstructorGeneratorTestsFixture() : tStruct(nullptr) {}

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CopyConstructorGenerator gen(parent, &tStruct);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateFieldValue(const t_field& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CopyConstructorGenerator gen(parent, &tStruct);
    gen.generateFieldValue(&f);
    return stream.str();
  }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CopyConstructorGenerator gen(parent, &tStruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  t_struct tStruct;
};

BOOST_FIXTURE_TEST_SUITE(CopyConstructorGeneratorTests, CopyConstructorGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_name_is_used_in_declaration) {
  tStruct.set_name("StructName");

  BOOST_CHECK_EQUAL(generateDeclaration(), "StructName(const StructName& other)");
}

BOOST_AUTO_TEST_CASE(empty_struct_shadows_argument_name_in_body) {
  BOOST_CHECK_EQUAL(generateBody(), "(void) other;\n");
}

BOOST_AUTO_TEST_CASE(field_generates_init_list_entry_only) {
  t_field f(nullptr, "f", 1);
  tStruct.append(&f);

  BOOST_CHECK(generateBody().empty());
  BOOST_CHECK_EQUAL(generateFieldValue(f), "other.f");
}

BOOST_AUTO_TEST_SUITE_END()
