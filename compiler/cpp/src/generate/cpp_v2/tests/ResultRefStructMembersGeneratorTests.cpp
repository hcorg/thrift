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

#include <parse/t_base_type.h>
#include <parse/t_field.h>
#include <parse/t_map.h>
#include <parse/t_struct.h>

#include "../ResultRefStructMembersGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ResultRefStructMembersGeneratorTestsFixture {

  ResultRefStructMembersGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateDefinitions() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ResultRefStructMembersGenerator gen(parent, &tstruct);
    gen.generateDefinitions();
    return stream.str();
  }

  std::string generateBodyDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ResultRefStructMembersGenerator gen(parent, &tstruct);
    gen.generateBodyDeclarations();
    return stream.str();
  }

  std::string generateFields() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ResultRefStructMembersGenerator gen(parent, &tstruct);
    gen.generateFields();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(ResultRefStructMembersGeneratorTests,
                         ResultRefStructMembersGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_generates_only_protocol_read_in_body) {
  BOOST_CHECK_EQUAL(generateBodyDeclarations(),
                    "std::uint32_t read(::apache::thrift::protocol::TProtocol* iprot);\n");
}

BOOST_AUTO_TEST_CASE(struct_generates_definitions_for_all_declared_methods) {
  BOOST_CHECK_EQUAL(
      generateDefinitions(),
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
      "std::ostream& operator<<(std::ostream& out, const StructName& obj) {\n"
      "  (void) obj;\n"
      "  using apache::thrift::to_string;\n"
      "  out << \"StructName(\";\n"
      "  out << \")\";\n"
      "  return out;\n"
      "}\n"
      "\n");
}

BOOST_AUTO_TEST_CASE(fields_are_generated_using_optionalreferences_when_needed) {
  t_field success(t_base_type::type_string, "success", 0);
  success.set_req(t_field::T_OPTIONAL);
  tstruct.append(&success);

  BOOST_CHECK_EQUAL(generateFields(), "boost::optional<std::string&> success; // 0\n");
}

BOOST_AUTO_TEST_SUITE_END()
