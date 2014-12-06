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

#include <parse/t_struct.h>

#include "../ArgsStructMembersGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ArgsStructMembersGeneratorTestsFixture {

  ArgsStructMembersGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateDefinitions() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsStructMembersGenerator gen(parent, &tstruct);
    gen.generateDefinitions();
    return stream.str();
  }

  std::string generateBodyDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsStructMembersGenerator gen(parent, &tstruct);
    gen.generateBodyDeclarations();
    return stream.str();
  }

  std::string generateStandaloneDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsStructMembersGenerator gen(parent, &tstruct);
    gen.generateStandaloneDeclarations();
    return stream.str();
  }

  std::string generateOtherNamespaceDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsStructMembersGenerator gen(parent, &tstruct);
    gen.generateOtherNamespaceDeclarations();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(ArgsStructMembersGeneratorTests, ArgsStructMembersGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_generates_constructor_and_protocol_functions_in_body) {
  BOOST_CHECK_EQUAL(generateBodyDeclarations(),
                    "StructName();\n"
                    "\n"
                    "void clear();\n"
                    "\n"
                    "std::uint32_t read(::apache::thrift::protocol::TProtocol* iprot);\n"
                    "std::uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;\n");
}

BOOST_AUTO_TEST_CASE(struct_generates_only_ostream_operator_as_standalone_function) {
  BOOST_CHECK_EQUAL(generateStandaloneDeclarations(),
                    "std::ostream& operator<<(std::ostream& out, const StructName& obj);\n");
}

BOOST_AUTO_TEST_CASE(struct_does_not_define_anything_outside_current_namespace) {
  BOOST_CHECK(generateOtherNamespaceDeclarations().empty());
}

BOOST_AUTO_TEST_CASE(struct_generates_definitions_for_all_declared_methods) {
  BOOST_CHECK_EQUAL(
      generateDefinitions(),
      "StructName::StructName()\n"
      "{\n"
      "}\n"
      "\n"
      "void StructName::clear() {\n"
      "}\n"
      "\n"
      "std::uint32_t StructName::read(::apache::thrift::protocol::TProtocol* iprot) {\n"
      "  std::uint32_t xfer = 0;\n"
      "  std::string fieldName;\n"
      "  ::apache::thrift::protocol::TType fieldType;\n"
      "  std::int16_t fieldId;\n"
      "\n"
      "  xfer += iprot->readStructBegin(fieldName);\n"
      "\n"
      "  while (true) {\n"
      "    xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);\n"
      "    if (fieldType == ::apache::thrift::protocol::T_STOP)\n"
      "      break;\n"
      "    xfer += iprot->skip(fieldType);\n"
      "    xfer += iprot->readFieldEnd();\n"
      "  }\n"
      "\n"
      "  xfer += iprot->readStructEnd();\n"
      "\n"
      "  return xfer;\n"
      "}\n"
      "\n"
      "std::uint32_t StructName::write(::apache::thrift::protocol::TProtocol* oprot) const {\n"
      "  std::uint32_t xfer = 0;\n"
      "  oprot->incrementRecursionDepth();\n"
      "  xfer += oprot->writeStructBegin(\"StructName\");\n"
      "\n"
      "  xfer += oprot->writeFieldStop();\n"
      "  xfer += oprot->writeStructEnd();\n"
      "  oprot->decrementRecursionDepth();\n"
      "  return xfer;\n"
      "}\n"
      "\n"
      "std::ostream& operator<<(std::ostream& out, const StructName& obj) {\n"
      "  (void) obj;\n"
      "  using apache::thrift::to_string;\n"
      "  out << \"StructName(\";\n"
      "  out << \")\";\n"
      "  return out;\n"
      "}\n"
      "\n");
}

BOOST_AUTO_TEST_SUITE_END()
