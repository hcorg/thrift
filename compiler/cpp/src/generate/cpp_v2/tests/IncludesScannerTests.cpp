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

#include <boost/test/unit_test.hpp>

#include "../IncludesScanner.h"

#include <parse/t_base_type.h>
#include <parse/t_struct.h>
#include <parse/t_enum.h>
#include <parse/t_list.h>
#include <parse/t_set.h>
#include <parse/t_map.h>
#include <parse/t_typedef.h>
#include <parse/t_service.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

struct IncludesScannerTestsFixture {
  IncludesScanner scanner;

  bool isNothingRequired() const {
    return !scanner.isCstdintRequired() && !scanner.isStringRequired()
           && !scanner.isVectorRequired() && !scanner.isSetRequired() && !scanner.isMapRequired()
           && scanner.getUsedTypes().empty();
  }
};

BOOST_FIXTURE_TEST_SUITE(IncludesScannerTests, IncludesScannerTestsFixture)

BOOST_AUTO_TEST_CASE(constructed_scanner_does_not_contain_any_includes) {
  BOOST_CHECK(isNothingRequired());
}

BOOST_AUTO_TEST_CASE(bool_type_does_not_require_any_header) {
  scanner.scan(t_base_type::type_bool);
  BOOST_CHECK(isNothingRequired());
}

BOOST_AUTO_TEST_CASE(double_type_does_not_require_any_header) {
  scanner.scan(t_base_type::type_double);
  BOOST_CHECK(isNothingRequired());
}

BOOST_AUTO_TEST_CASE(byte_type_requires_cstdint_header) {
  scanner.scan(t_base_type::type_byte);
  BOOST_CHECK(scanner.isCstdintRequired());
}

BOOST_AUTO_TEST_CASE(i16_type_requires_cstdint_header) {
  scanner.scan(t_base_type::type_i16);
  BOOST_CHECK(scanner.isCstdintRequired());
}

BOOST_AUTO_TEST_CASE(i32_type_requires_cstdint_header) {
  scanner.scan(t_base_type::type_i32);
  BOOST_CHECK(scanner.isCstdintRequired());
}

BOOST_AUTO_TEST_CASE(i64_type_requires_cstdint_header) {
  scanner.scan(t_base_type::type_i64);
  BOOST_CHECK(scanner.isCstdintRequired());
}

BOOST_AUTO_TEST_CASE(void_type_does_not_require_header) {
  scanner.scan(t_base_type::type_void);
  BOOST_CHECK(isNothingRequired());
}

BOOST_AUTO_TEST_CASE(string_type_requires_string_header) {
  scanner.scan(t_base_type::type_string);
  BOOST_CHECK(scanner.isStringRequired());
}

BOOST_AUTO_TEST_CASE(struct_type_requires_type_header) {
  t_struct s(nullptr, "StructName");

  scanner.scan(&s);

  BOOST_REQUIRE_EQUAL(scanner.getUsedTypes().size(), 1u);
  BOOST_CHECK_EQUAL(*(scanner.getUsedTypes().begin()), "StructName");
}

BOOST_AUTO_TEST_CASE(enum_type_requires_type_header) {
  t_enum e(nullptr);
  e.set_name("EnumName");

  scanner.scan(&e);

  BOOST_REQUIRE_EQUAL(scanner.getUsedTypes().size(), 1u);
  BOOST_CHECK_EQUAL(*(scanner.getUsedTypes().begin()), "EnumName");
}

BOOST_AUTO_TEST_CASE(repeated_scan_does_not_repeat_found_headers) {
  t_enum e(nullptr);
  e.set_name("EnumName");

  scanner.scan(&e);
  scanner.scan(&e);

  BOOST_CHECK_EQUAL(scanner.getUsedTypes().size(), 1u);
}

BOOST_AUTO_TEST_CASE(multiple_types_requires_multiple_headers) {
  t_enum e(nullptr);
  e.set_name("EnumName");
  t_struct s(nullptr, "StructName");

  scanner.scan(&e);
  scanner.scan(&s);

  BOOST_CHECK_EQUAL(scanner.getUsedTypes().size(), 2u);
}

BOOST_AUTO_TEST_CASE(list_type_require_vector_header) {
  t_list l(t_base_type::type_bool);

  scanner.scan(&l);

  BOOST_CHECK(scanner.isVectorRequired());
}

BOOST_AUTO_TEST_CASE(list_item_type_is_scanned_for_headers) {
  t_list l(t_base_type::type_string);

  scanner.scan(&l);

  BOOST_CHECK(scanner.isVectorRequired());
  BOOST_CHECK(scanner.isStringRequired());
}

BOOST_AUTO_TEST_CASE(set_type_require_set_header) {
  t_set s(t_base_type::type_bool);

  scanner.scan(&s);

  BOOST_CHECK(scanner.isSetRequired());
}

BOOST_AUTO_TEST_CASE(set_item_type_is_scanner_for_headers) {
  t_list l(t_base_type::type_string);
  t_set s(&l);

  scanner.scan(&s);

  BOOST_CHECK(scanner.isSetRequired());
  BOOST_CHECK(scanner.isVectorRequired());
  BOOST_CHECK(scanner.isStringRequired());
}

BOOST_AUTO_TEST_CASE(map_type_requires_map_header) {
  t_map m(t_base_type::type_double, t_base_type::type_bool);

  scanner.scan(&m);

  BOOST_CHECK(scanner.isMapRequired());
}

BOOST_AUTO_TEST_CASE(map_key_and_value_types_are_scanned_for_headers) {
  t_map m(t_base_type::type_string, t_base_type::type_i32);

  scanner.scan(&m);

  BOOST_CHECK(scanner.isMapRequired());
  BOOST_CHECK(scanner.isStringRequired());
  BOOST_CHECK(scanner.isCstdintRequired());
}

BOOST_AUTO_TEST_CASE(exception_type_requires_type_header) {
  t_struct s(nullptr, "ExceptionName");
  s.set_xception(true);

  scanner.scan(&s);

  BOOST_REQUIRE_EQUAL(scanner.getUsedTypes().size(), 1u);
  BOOST_CHECK_EQUAL(*(scanner.getUsedTypes().begin()), "ExceptionName");
}

BOOST_AUTO_TEST_CASE(service_type_requires_service_header) {
  t_service s(nullptr);
  s.set_name("ServiceName");

  scanner.scan(&s);

  BOOST_REQUIRE_EQUAL(scanner.getUsedTypes().size(), 1u);
  BOOST_CHECK_EQUAL(*(scanner.getUsedTypes().begin()), "ServiceName");
}

BOOST_AUTO_TEST_CASE(typedef_type_requires_type_header_only) {
  t_typedef t(nullptr, t_base_type::type_string, "TypedefedType");

  scanner.scan(&t);

  BOOST_REQUIRE_EQUAL(scanner.getUsedTypes().size(), 1u);
  BOOST_CHECK_EQUAL(*(scanner.getUsedTypes().begin()), "TypedefedType");
  BOOST_CHECK(!scanner.isStringRequired());
}

BOOST_AUTO_TEST_SUITE_END()
