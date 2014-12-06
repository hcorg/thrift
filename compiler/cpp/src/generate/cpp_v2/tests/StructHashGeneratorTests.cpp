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

#include "../StructHashGenerator.h"

#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct StructHashGeneratorTestFixture {

  StructHashGeneratorTestFixture() : tstruct(nullptr) {}

  std::string generate() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructHashGenerator gen(parent, &tstruct);
    gen.generateOperatorBody();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(StructHashGeneratorTest, StructHashGeneratorTestFixture)

BOOST_AUTO_TEST_CASE(empty_struct_returns_zero_hash) {
  BOOST_CHECK_EQUAL(generate(),
                    "(void) obj;\n"
                    "return 0;\n");
}

BOOST_AUTO_TEST_CASE(not_empty_struct_combines_hashes_of_items) {
  t_field f1(nullptr, "f1", 1);
  tstruct.append(&f1);
  t_field f2(nullptr, "f2", 2);
  tstruct.append(&f2);

  BOOST_CHECK_EQUAL(generate(),
                    "using apache::thrift::hashCombine;\n"
                    "std::size_t h = 0;\n"
                    "hashCombine(h, obj.f1);\n"
                    "hashCombine(h, obj.f2);\n"
                    "return h;\n");
}

BOOST_AUTO_TEST_CASE(optional_field_adds_to_hash_only_if_contains_value) {
  t_field f1(nullptr, "f1", 1);
  f1.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generate(),
                    "using apache::thrift::hashCombine;\n"
                    "std::size_t h = 0;\n"
                    "if (obj.f1)\n"
                    "  hashCombine(h, *obj.f1);\n"
                    "return h;\n");
}

BOOST_AUTO_TEST_SUITE_END()
