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

#include "../ScopeGuard.h"
#include "../IndentKeeper.h"
#include "../OutputGenerator.h"

using namespace apache::thrift::compiler::generate;

struct ScopeGuardTestsFixture {
  std::ostringstream stream;
  OutputGenerator generator;

  ScopeGuardTestsFixture() : generator(stream) {}

  std::string text() const { return stream.str(); }
};

BOOST_FIXTURE_TEST_SUITE(ScopeGuardTests, ScopeGuardTestsFixture)

BOOST_AUTO_TEST_CASE(ScopeGuard_open_bracket_and_increses_indent_on_construction) {
  const int startIndent = generator.getIndent();

  ScopeGuard guard(generator);

  BOOST_CHECK_EQUAL(text(), " {\n");
  BOOST_CHECK_EQUAL(generator.getIndent(), startIndent + 1);
}

BOOST_AUTO_TEST_CASE(ScopeGuard_closes_bracket_and_restores_indent_on_destruction) {
  const int startIndent = generator.getIndent();

  { ScopeGuard guard(generator); }

  BOOST_CHECK_EQUAL(text(), " {\n}\n");
  BOOST_CHECK_EQUAL(generator.getIndent(), startIndent);
}

BOOST_AUTO_TEST_SUITE_END()
