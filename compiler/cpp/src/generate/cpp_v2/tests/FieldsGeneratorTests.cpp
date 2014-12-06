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

#include "../FieldsGenerator.h"

#include <parse/t_enum.h>
#include <parse/t_base_type.h>
#include <parse/t_field.h>
#include <parse/t_list.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct FieldsGeneratorTestsFixture {

  FieldsGeneratorTestsFixture() : currentNamespace("some.name"), enumType(nullptr) {}

  ~FieldsGeneratorTestsFixture() { clear(); }

  std::string generated() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    FieldsGenerator gen(parent, fields_, currentNamespace);
    gen.generateContents();
    return stream.str();
  }

  std::string header() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    FieldsGenerator gen(parent, fields_, currentNamespace);
    gen.generateHeader();
    return stream.str();
  }

  void clear() {
    for (auto& f : fields_)
      delete f;
    fields_.clear();
  }

  void addField(t_type* type, const std::string& name = "fieldName", std::int32_t key = 1) {
    fields_.push_back(new t_field(type, name, key));
  }

  void addOptionalField(t_type* type, const std::string& name = "fieldName", std::int32_t key = 1) {
    addField(type, name, key);
    fields_.back()->set_req(t_field::T_OPTIONAL);
  }

  std::vector<t_field*> fields_;

  const std::string currentNamespace;

  t_enum enumType;
};

BOOST_FIXTURE_TEST_SUITE(FieldsGeneratorTests, FieldsGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(no_fields_generate_no_output) {
  BOOST_CHECK(header().empty());
  BOOST_CHECK(generated().empty());
}

BOOST_AUTO_TEST_CASE(void_field_causes_throw) {
  addField(t_base_type::type_void);

  BOOST_CHECK_THROW(header(), std::string);
}

BOOST_AUTO_TEST_CASE(type_headers_aggregate) {
  enumType.set_name("EnumName");
  addField(&enumType);
  addField(t_base_type::type_string);

  BOOST_CHECK_EQUAL(header(),
                    "#include <string>\n"
                    "#include \"EnumName.h\"\n");
}

BOOST_AUTO_TEST_CASE(base_types_generates_properly) {
  addField(t_base_type::type_string);
  BOOST_CHECK_EQUAL(generated(), "std::string fieldName; // 1\n");
}

BOOST_AUTO_TEST_CASE(field_name_and_key_propagates_properly) {
  addField(t_base_type::type_i64, "field1", 1);
  addField(t_base_type::type_i64, "fieldA", 10);
  addField(t_base_type::type_i64, "field_D", 200);

  BOOST_CHECK_EQUAL(generated(),
                    "std::int64_t field1; // 1\n"
                    "std::int64_t fieldA; // 10\n"
                    "std::int64_t field_D; // 200\n");
}

BOOST_AUTO_TEST_CASE(optional_fields_require_boost_optional) {
  addOptionalField(t_base_type::type_bool);

  BOOST_CHECK_EQUAL(header(), "#include <boost/optional.hpp>\n");
}

BOOST_AUTO_TEST_CASE(optional_fields_generate_boost_optional) {
  addOptionalField(t_base_type::type_bool);

  BOOST_CHECK_EQUAL(generated(), "boost::optional<bool> fieldName; // 1\n");
}

BOOST_AUTO_TEST_CASE(user_defined_fields_generate_properly) {
  enumType.set_name("EnumName");
  addField(&enumType);

  BOOST_CHECK_EQUAL(generated(), "EnumName fieldName; // 1\n");
}

BOOST_AUTO_TEST_CASE(collection_generate_properly) {
  enumType.set_name("EnumName");
  t_list l(&enumType);
  addField(&l);

  BOOST_CHECK_EQUAL(generated(), "std::vector<EnumName> fieldName; // 1\n");
}

BOOST_AUTO_TEST_SUITE_END()
