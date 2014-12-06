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

#include "../StructConstructorGenerator.h"

#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

namespace {

class DummyGenerator : public StructConstructorGenerator {
public:
  DummyGenerator(OutputGenerator& parent, const t_struct* ts)
    : StructConstructorGenerator(parent, ts) {}

  void generateArguments() override { out() << "Arg arg"; }

  void generateDefinitionBody() override { indented() << "dummy_contents;" << endl; }

  void generateFieldValue(const t_field* f) override { out() << f->get_name() << "_value"; }
};

} // namespace

struct StructConstructorGeneratorTestsFixture {

  StructConstructorGeneratorTestsFixture() : tStruct(nullptr) { tStruct.set_name("StructName"); }

  std::string generateDefinition() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    DummyGenerator gen(parent, &tStruct);
    gen.generateDefinition();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    DummyGenerator gen(parent, &tStruct);
    gen.generateDeclaration();
    return stream.str();
  }

  t_struct tStruct;
};

BOOST_FIXTURE_TEST_SUITE(StructConstructorGeneratorTests, StructConstructorGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_name_and_construct_arguments_are_used_in_declaration) {
  BOOST_CHECK_EQUAL(generateDeclaration(), "StructName(Arg arg)");
}

BOOST_AUTO_TEST_CASE(empty_struct_generates_basic_constructor) {
  BOOST_CHECK_EQUAL(generateDefinition(),
                    "StructName::StructName(Arg arg)\n"
                    "{\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(single_field_struct_initializes_that_field) {
  t_field f(nullptr, "f", 1);
  f.set_req(t_field::T_OPTIONAL);
  tStruct.append(&f);

  BOOST_CHECK_EQUAL(generateDefinition(),
                    "StructName::StructName(Arg arg)\n"
                    "  : f{f_value}\n"
                    "{\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(empty_exception_initializes_texception_with_exception_name) {
  tStruct.set_xception(true);
  BOOST_CHECK_EQUAL(generateDefinition(),
                    "StructName::StructName(Arg arg)\n"
                    "  : ::apache::thrift::TException{\"StructName - Thrift Generated\"}\n"
                    "{\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(single_field_exception_initializes_that_field_after_texception) {
  t_field f(nullptr, "f", 1);
  f.set_req(t_field::T_OPTIONAL);
  tStruct.append(&f);
  tStruct.set_xception(true);

  BOOST_CHECK_EQUAL(generateDefinition(),
                    "StructName::StructName(Arg arg)\n"
                    "  : ::apache::thrift::TException{\"StructName - Thrift Generated\"},\n"
                    "    f{f_value}\n"
                    "{\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_CASE(multi_field_struct_initializes_all_fields) {
  t_field f1(nullptr, "f1", 1);
  tStruct.append(&f1);
  t_field f2(nullptr, "f2", 2);
  tStruct.append(&f2);
  t_field f3(nullptr, "f3", 3);
  tStruct.append(&f3);

  BOOST_CHECK_EQUAL(generateDefinition(),
                    "StructName::StructName(Arg arg)\n"
                    "  : f1{f1_value},\n"
                    "    f2{f2_value},\n"
                    "    f3{f3_value}\n"
                    "{\n"
                    "  dummy_contents;\n"
                    "}\n");
}

BOOST_AUTO_TEST_SUITE_END()
