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

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include "../StructDefinitionGenerator.h"

#include <parse/t_struct.h>

#include "StructMembersGeneratorStub.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct StructDefinitionGeneratorTestsFixture {

  StructDefinitionGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateHeader() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    auto factory = StructMembersGeneratorFactoryPtr(new StructMembersGeneratorStubFactory);
    StructDefinitionGenerator gen(parent, std::move(factory), &tstruct);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateContents() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    auto factory = StructMembersGeneratorFactoryPtr(new StructMembersGeneratorStubFactory);
    StructDefinitionGenerator gen(parent, std::move(factory), &tstruct);
    gen.generateContents();
    return stream.str();
  }

  std::string generateFooter() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    auto factory = StructMembersGeneratorFactoryPtr(new StructMembersGeneratorStubFactory);
    StructDefinitionGenerator gen(parent, std::move(factory), &tstruct);
    gen.generateFooter();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(StructDefinitionGeneratorTests, StructDefinitionGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_definition_does_not_generate_footer) {
  BOOST_CHECK(generateFooter().empty());
}

BOOST_AUTO_TEST_CASE(default_struct_definition_does_not_generate_header) {
  BOOST_CHECK(generateHeader().empty());
}

BOOST_AUTO_TEST_CASE(struct_definition_generates_members_definitons) {
  BOOST_CHECK_EQUAL(generateContents(), "StructName: definitions\n");
}

BOOST_AUTO_TEST_SUITE_END()
