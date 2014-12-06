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
#include <string>

#include <boost/test/unit_test.hpp>

#include "../MethodGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

namespace {

class DummyGenerator : public MethodGenerator {
public:
  DummyGenerator(OutputGenerator& parent) : MethodGenerator(parent, "ClassName") {}

  void generateReturnType() override { out() << "ReturnType"; }

  void generateDeclarationWithoutReturn() override { out() << "methodName(Arg arg)"; }

  void generateDefinitionBody() override { indented() << "dummy_contents;\n"; }
};

} // namespace

struct MethodGeneratorTestsFixture {

  MethodGeneratorTestsFixture() : parentGen(stream), generator(parentGen) {}

  std::string generateDefinition() {
    generator.generateDefinition();
    return stream.str();
  }

  std::string generateDeclaration() {
    generator.generateDeclaration();
    return stream.str();
  }

  std::ostringstream stream;
  OutputGenerator parentGen;
  DummyGenerator generator;
};

BOOST_FIXTURE_TEST_SUITE(MethodGeneratorTests, MethodGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(method_declaration_uses_return_type_and_method_name) {
  BOOST_CHECK_EQUAL(generateDeclaration(), "ReturnType methodName(Arg arg)");
}

BOOST_AUTO_TEST_CASE(method_definition_contains_class_name) {
  BOOST_CHECK_EQUAL(generateDefinition(),
                    "ReturnType ClassName::methodName(Arg arg) {\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(virtual_function_declaration_begins_with_modifier) {
  generator.setType(MethodGenerator::Type::Virtual);
  BOOST_CHECK_EQUAL(generateDeclaration(), "virtual ReturnType methodName(Arg arg)");
}

BOOST_AUTO_TEST_CASE(virtual_method_definition_does_not_differ_from_standard_one) {
  generator.setType(MethodGenerator::Type::Virtual);
  BOOST_CHECK_EQUAL(generateDefinition(),
                    "ReturnType ClassName::methodName(Arg arg) {\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(override_function_declaration_ends_with_modifier) {
  generator.setType(MethodGenerator::Type::Override);
  BOOST_CHECK_EQUAL(generateDeclaration(), "ReturnType methodName(Arg arg) override");
}

BOOST_AUTO_TEST_CASE(override_method_definition_does_not_differ_from_standard_one) {
  generator.setType(MethodGenerator::Type::Override);
  BOOST_CHECK_EQUAL(generateDefinition(),
                    "ReturnType ClassName::methodName(Arg arg) {\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(pure_virtual_function_declaration_begins_with_modifier_and_ends_with_zero) {
  generator.setType(MethodGenerator::Type::PureVirtual);
  BOOST_CHECK_EQUAL(generateDeclaration(), "virtual ReturnType methodName(Arg arg) = 0");
}

BOOST_AUTO_TEST_CASE(pure_virtual_method_definition_should_not_be_generated) {
  generator.setType(MethodGenerator::Type::PureVirtual);
  BOOST_CHECK_THROW(generateDefinition(), std::string);
}

BOOST_AUTO_TEST_SUITE_END()
