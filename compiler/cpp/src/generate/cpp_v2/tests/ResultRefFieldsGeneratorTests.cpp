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

#include "../ResultRefFieldsGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ResultRefFieldsGeneratorTestsFixture {

  ResultRefFieldsGeneratorTestsFixture() : currentNamespace("some.name") {}

  ~ResultRefFieldsGeneratorTestsFixture() { clear(); }

  std::string generated() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ResultRefFieldsGenerator gen(parent, fields_, currentNamespace);
    gen.generateContents();
    return stream.str();
  }

  void clear() {
    for (auto& f : fields_)
      delete f;
    fields_.clear();
  }

  void addField(t_type* type, const std::string& name = "argName", std::int32_t key = 1) {
    auto f = new t_field(type, name, key);
    f->set_req(t_field::T_OPTIONAL);
    fields_.push_back(f);
  }

  void addSpecialField(t_type* type) { addField(type, "success", 0); }

  std::vector<t_field*> fields_;

  const std::string currentNamespace;
};

BOOST_FIXTURE_TEST_SUITE(ResultRefFieldsGeneratorTests, ResultRefFieldsGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(single_simple_arg_generates_properly) {
  addField(t_base_type::type_i64);
  BOOST_CHECK_EQUAL(generated(), "boost::optional<std::int64_t> argName; // 1\n");
}

BOOST_AUTO_TEST_CASE(simple_special_field_generates_normal_optional) {
  addSpecialField(t_base_type::type_double);
  BOOST_CHECK_EQUAL(generated(), "boost::optional<double> success; // 0\n");
}

BOOST_AUTO_TEST_CASE(complex_special_field_generates_normal_optional) {
  addSpecialField(t_base_type::type_string);
  BOOST_CHECK_EQUAL(generated(), "boost::optional<std::string&> success; // 0\n");
}

BOOST_AUTO_TEST_SUITE_END()
