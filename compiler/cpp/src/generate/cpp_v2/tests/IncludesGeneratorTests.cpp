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

#include "../IncludesScanner.h"
#include "../IncludesGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

namespace {

struct DummyScanner : public IncludesScanner {
  void requireCstdint() { cstdintRequired_ = true; }
  void requireString() { stringRequired_ = true; }
  void requireVector() { vectorRequired_ = true; }
  void requireSet() { setRequired_ = true; }
  void requireMap() { mapRequired_ = true; }

  void addRequiredType(const std::string& name) { addUsedType(name); }
};

struct Fixture {

  DummyScanner scanner;

  std::string generated() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    IncludesGenerator gen(parent);
    gen.generate(scanner);
    return stream.str();
  }
};

BOOST_FIXTURE_TEST_SUITE(IncludesGeneratorTests, Fixture)

BOOST_AUTO_TEST_CASE(when_nothing_in_scanner_nothing_is_generated) {
  BOOST_CHECK(generated().empty());
}

BOOST_AUTO_TEST_CASE(when_cstdint_is_required_it_is_generated) {
  scanner.requireCstdint();

  BOOST_CHECK_EQUAL(generated(), "#include <cstdint>\n");
}

BOOST_AUTO_TEST_CASE(when_string_is_required_it_is_generated) {
  scanner.requireString();

  BOOST_CHECK_EQUAL(generated(), "#include <string>\n");
}

BOOST_AUTO_TEST_CASE(when_vector_is_required_it_is_generated) {
  scanner.requireVector();

  BOOST_CHECK_EQUAL(generated(), "#include <vector>\n");
}

BOOST_AUTO_TEST_CASE(when_set_is_required_it_is_generated) {
  scanner.requireSet();

  BOOST_CHECK_EQUAL(generated(), "#include <unordered_set>\n");
}

BOOST_AUTO_TEST_CASE(when_map_is_required_it_is_generated) {
  scanner.requireMap();

  BOOST_CHECK_EQUAL(generated(), "#include <unordered_map>\n");
}

BOOST_AUTO_TEST_CASE(added_required_type_is_generated) {
  scanner.addRequiredType("SomeType");

  BOOST_CHECK_EQUAL(generated(), "#include \"SomeType.h\"\n");
}

BOOST_AUTO_TEST_CASE(required_type_added_twice_is_generated_once) {
  scanner.addRequiredType("SomeType");
  scanner.addRequiredType("SomeType");

  BOOST_CHECK_EQUAL(generated(), "#include \"SomeType.h\"\n");
}

BOOST_AUTO_TEST_CASE(added_three_required_types_are_generated_and_sorted) {
  scanner.addRequiredType("SomeType2");
  scanner.addRequiredType("SomeType1");
  scanner.addRequiredType("SomeType3");

  BOOST_CHECK_EQUAL(generated(),
                    "#include \"SomeType1.h\"\n"
                    "#include \"SomeType2.h\"\n"
                    "#include \"SomeType3.h\"\n");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace
