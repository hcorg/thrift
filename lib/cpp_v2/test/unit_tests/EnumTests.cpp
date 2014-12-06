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

#include <cstdint>

#include <boost/test/auto_unit_test.hpp>

#include "gen-cpp/MyEnum1.h"
#include "gen-cpp/MyEnum2.h"
#include "gen-cpp/MyEnum3.h"
#include "gen-cpp/MyEnum4.h"
#include "gen-cpp/MyEnumStruct.h"

static_assert(static_cast<std::int32_t>(MyEnum1::ME1_0) == 0, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum1::ME1_1) == 1, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum1::ME1_2) == 2, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum1::ME1_3) == 3, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum1::ME1_5) == 5, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum1::ME1_6) == 6, "Enum value mismatch");

static_assert(static_cast<std::int32_t>(MyEnum2::ME2_0) == 0, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum2::ME2_1) == 1, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum2::ME2_2) == 2, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_0) == 0, "Enum value mismatch");

static_assert(static_cast<std::int32_t>(MyEnum3::ME3_1) == 1, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_N2) == -2, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_N1) == -1, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_D0) == 0, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_D1) == 1, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_9) == 9, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum3::ME3_10) == 10, "Enum value mismatch");

static_assert(static_cast<std::int32_t>(MyEnum4::ME4_A) == 0x7ffffffd, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum4::ME4_B) == 0x7ffffffe, "Enum value mismatch");
static_assert(static_cast<std::int32_t>(MyEnum4::ME4_C) == 0x7fffffff, "Enum value mismatch");

BOOST_AUTO_TEST_SUITE(EnumTest)

BOOST_AUTO_TEST_CASE(test_enum_constant) {
  MyEnumStruct ms;
  BOOST_CHECK_EQUAL(static_cast<std::int32_t>(ms.me2_2), 2);
  BOOST_CHECK_EQUAL(static_cast<std::int32_t>(ms.me3_n2), -2);
  BOOST_CHECK_EQUAL(static_cast<std::int32_t>(ms.me3_d1), 1);
}

BOOST_AUTO_TEST_SUITE_END()
