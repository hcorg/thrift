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
#include <parse/t_base_type.h>

#include "../CompleteStructMembersGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct CompleteStructMembersGeneratorTestsFixture {

  CompleteStructMembersGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateDefinitions() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CompleteStructMembersGenerator gen(parent, &tstruct);
    gen.generateDefinitions();
    return stream.str();
  }

  std::string generateBodyDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CompleteStructMembersGenerator gen(parent, &tstruct);
    gen.generateBodyDeclarations();
    return stream.str();
  }

  std::string generateStandaloneDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CompleteStructMembersGenerator gen(parent, &tstruct);
    gen.generateStandaloneDeclarations();
    return stream.str();
  }

  std::string generateOtherNamespaceDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CompleteStructMembersGenerator gen(parent, &tstruct);
    gen.generateOtherNamespaceDeclarations();
    return stream.str();
  }

  std::string generateFields() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    CompleteStructMembersGenerator gen(parent, &tstruct);
    gen.generateFields();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(CompleteStructMembersGeneratorTests,
                         CompleteStructMembersGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_generates_constructors_assignments_and_protocol_functions_in_body) {
  BOOST_CHECK_EQUAL(generateBodyDeclarations(),
                    "StructName();\n"
                    "StructName(const StructName& other);\n"
                    "StructName(StructName&& other);\n"
                    "\n"
                    "StructName& operator=(const StructName& other);\n"
                    "StructName& operator=(StructName&& other);\n"
                    "\n"
                    "void clear();\n"
                    "\n"
                    "std::uint32_t read(::apache::thrift::protocol::TProtocol* iprot);\n"
                    "std::uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;\n");
}

BOOST_AUTO_TEST_CASE(struct_generates_operators_as_standalone_functions) {
  BOOST_CHECK_EQUAL(
      generateStandaloneDeclarations(),
      "bool operator==(const StructName& a, const StructName& b);\n"
      // TODO enable when std::map/set "bool operator<(const StructName& a, const StructName& b);\n"
      "void swap(StructName& a, StructName& b);\n"
      "std::ostream& operator<<(std::ostream& out, const StructName& obj);\n"
      "inline bool operator!=(const StructName& a, const StructName& b) { return !(a == b); }\n");
}

BOOST_AUTO_TEST_CASE(struct_defines_hash_outside_current_namespace) {
  BOOST_CHECK_EQUAL(generateOtherNamespaceDeclarations(),
                    "namespace std {\n"
                    "template <>\n"
                    "struct hash<StructName> {\n"
                    "  std::size_t operator()(const StructName& obj) const {\n"
                    "    (void) obj;\n"
                    "    return 0;\n"
                    "  }\n"
                    "};\n"
                    "} // namespace std\n");
}

BOOST_AUTO_TEST_CASE(struct_generates_definitions_for_all_declared_methods) {
  BOOST_CHECK_EQUAL(
      generateDefinitions(),
      "StructName::StructName()\n"
      "{\n"
      "}\n"
      "\n"
      "StructName::StructName(const StructName& other)\n"
      "{\n"
      "  (void) other;\n"
      "}\n"
      "\n"
      "StructName::StructName(StructName&& other)\n"
      "{\n"
      "  (void) other;\n"
      "}\n"
      "\n"
      "StructName& StructName::operator=(const StructName& other) {\n"
      "  (void) other;\n"
      "  return *this;\n"
      "}\n"
      "\n"
      "StructName& StructName::operator=(StructName&& other) {\n"
      "  (void) other;\n"
      "  return *this;\n"
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
      "bool operator==(const StructName& a, const StructName& b) {\n"
      "  (void) a;\n"
      "  (void) b;\n"
      "  return true;\n"
      "}\n"
      "\n"
      // TODO enable      // "bool operator<(const StructName& a, const StructName& b) {\n"
      // "  return false;\n"
      // "}\n"
      // "\n"
      "void swap(StructName& a, StructName& b) {\n"
      "  using ::std::swap;\n"
      "  (void) a;\n"
      "  (void) b;\n"
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

BOOST_AUTO_TEST_CASE(struct_with_fields_generates_them) {
  t_field f(t_base_type::type_i32, "field", 1);
  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateFields(), "std::int32_t field; // 1\n");
}

BOOST_AUTO_TEST_SUITE_END()
