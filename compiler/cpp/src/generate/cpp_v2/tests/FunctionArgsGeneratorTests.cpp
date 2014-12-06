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
#include <vector>
#include <cstdint>

#include <boost/test/unit_test.hpp>

#include "../FunctionArgsGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct FunctionArgsGeneratorTestsFixture {

  FunctionArgsGeneratorTestsFixture() : currentNamespace("some.name") {}

  ~FunctionArgsGeneratorTestsFixture() { clear(); }

  std::string generated() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    FunctionArgsGenerator gen(parent, fields_, currentNamespace);
    gen.generateContents();
    return stream.str();
  }

  std::string header() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    FunctionArgsGenerator gen(parent, fields_, currentNamespace);
    gen.generateHeader();
    return stream.str();
  }

  void clear() {
    for (auto& f : fields_)
      delete f;
    fields_.clear();
  }

  void addField(t_type* type, const std::string& name = "argName", std::int32_t key = 1) {
    fields_.push_back(new t_field(type, name, key));
  }

  std::vector<t_field*> fields_;

  const std::string currentNamespace;
};

BOOST_FIXTURE_TEST_SUITE(FunctionArgsGeneratorTests, FunctionArgsGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(no_fields_generate_no_output) {
  BOOST_CHECK(header().empty());
  BOOST_CHECK(generated().empty());
}

BOOST_AUTO_TEST_CASE(single_arg_generates_properly) {
  addField(t_base_type::type_i64);
  BOOST_CHECK_EQUAL(generated(), "std::int64_t argName");
}

BOOST_AUTO_TEST_CASE(multiple_args_generates_properly) {
  addField(t_base_type::type_i64, "arg1", 1);
  addField(t_base_type::type_bool, "arg2", 2);
  BOOST_CHECK_EQUAL(generated(), "std::int64_t arg1, bool arg2");
}

BOOST_AUTO_TEST_CASE(complex_types_are_passed_as_references) {
  addField(t_base_type::type_string, "arg1", 1);
  BOOST_CHECK_EQUAL(generated(), "const std::string& arg1");
}

BOOST_AUTO_TEST_SUITE_END()
