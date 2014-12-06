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

#include "../EnumGenerator.h"

#include <parse/t_enum.h>
#include <parse/t_enum_value.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct EnumGeneratorTestsFixture {
  EnumGeneratorTestsFixture()
    : enum_(nullptr), value1_("Name1", 1), value2_("Name2", 10), value3_("Name3", 3) {}

  std::string generated() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    EnumGenerator gen(parent, &enum_);
    gen.generateContents();
    return stream.str();
  }

  std::string header() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    EnumGenerator gen(parent, &enum_);
    gen.generateHeader();
    return stream.str();
  }

  std::string footer() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    EnumGenerator gen(parent, &enum_);
    gen.generateFooter();
    return stream.str();
  }

  t_enum enum_;
  t_enum_value value1_;
  t_enum_value value2_;
  t_enum_value value3_;
};

BOOST_FIXTURE_TEST_SUITE(EnumGeneratorTests, EnumGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(empty_enum_generated_properly) {
  enum_.set_name("Test1");
  BOOST_CHECK_EQUAL(generated(),
                    "enum class Test1 : std::int32_t {\n"
                    "};\n");
}

BOOST_AUTO_TEST_CASE(proper_headers_included) {
  BOOST_CHECK_EQUAL(header(),
                    "#include <cstdint>\n"
                    "#include <functional>\n");
}

BOOST_AUTO_TEST_CASE(single_value_enum_generated_properly) {
  enum_.set_name("Test2");
  enum_.append(&value1_);

  BOOST_CHECK_EQUAL(generated(),
                    "enum class Test2 : std::int32_t {\n"
                    "  Name1 = 1,\n"
                    "};\n");
}

BOOST_AUTO_TEST_CASE(multiple_value_enum_generated_properly) {
  enum_.set_name("Test3");
  enum_.append(&value1_);
  enum_.append(&value3_);
  enum_.append(&value2_);

  BOOST_CHECK_EQUAL(generated(),
                    "enum class Test3 : std::int32_t {\n"
                    "  Name1 = 1,\n"
                    "  Name3 = 3,\n"
                    "  Name2 = 10,\n"
                    "};\n");
}

BOOST_AUTO_TEST_CASE(footer_contains_hash_definition) {
  enum_.set_name("EnumName");
  BOOST_CHECK_EQUAL(footer(),
                    "namespace std {\n"
                    "template <>\n"
                    "struct hash<EnumName> {\n"
                    "  std::size_t operator()(const EnumName& obj) const {\n"
                    "    std::hash<std::int32_t> hasher;\n"
                    "    return hasher(static_cast<std::int32_t>(obj));\n"
                    "  }\n"
                    "};\n"
                    "} // namespace std\n");
}

BOOST_AUTO_TEST_SUITE_END()
