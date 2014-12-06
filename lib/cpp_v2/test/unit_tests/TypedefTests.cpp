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
#include <string>
#include <type_traits>

#include "gen-cpp/MyInt32.h"
#include "gen-cpp/MyString.h"
#include "gen-cpp/MyStruct.h"
#include "gen-cpp/TypedefTestStruct.h"

static_assert(std::is_same<std::int32_t, thrift::test::MyInt32>::value,
              "MyInt32 is not std::int32_t");
static_assert(std::is_same<std::string, thrift::test::MyString>::value,
              "MyString is not std::string");
static_assert(std::is_same<thrift::test::TypedefTestStruct, thrift::test::MyStruct>::value,
              "MyStruct is not TypedefTestStruct");
