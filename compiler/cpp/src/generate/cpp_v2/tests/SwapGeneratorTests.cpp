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

#include "../SwapGenerator.h"

#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct SwapGeneratorTestsFixture {

  SwapGeneratorTestsFixture() : tstruct(nullptr) {}

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SwapGenerator gen(parent, &tstruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    SwapGenerator gen(parent, &tstruct);
    gen.generateDeclaration();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(SwapGeneratorTests, SwapGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(proper_declaration_is_generated) {
  tstruct.set_name("StructName");
  BOOST_CHECK_EQUAL(generateDeclaration(), "void swap(StructName& a, StructName& b)");
}

BOOST_AUTO_TEST_CASE(empty_struct_generates_noop_body) {
  BOOST_CHECK_EQUAL(generateBody(),
                    "using ::std::swap;\n"
                    "(void) a;\n"
                    "(void) b;\n");
}

BOOST_AUTO_TEST_CASE(single_field_generates_single_swap) {
  t_field f(nullptr, "f1", 1);
  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "using ::std::swap;\n"
                    "swap(a.f1, b.f1);\n");
}

BOOST_AUTO_TEST_CASE(multiple_fields_generates_single_swaps) {
  t_field f1(nullptr, "f1", 1);
  tstruct.append(&f1);
  t_field f2(nullptr, "f2", 2);
  tstruct.append(&f2);
  t_field f3(nullptr, "f3", 3);
  tstruct.append(&f3);

  BOOST_CHECK_EQUAL(generateBody(),
                    "using ::std::swap;\n"
                    "swap(a.f1, b.f1);\n"
                    "swap(a.f2, b.f2);\n"
                    "swap(a.f3, b.f3);\n");
}

BOOST_AUTO_TEST_SUITE_END()
