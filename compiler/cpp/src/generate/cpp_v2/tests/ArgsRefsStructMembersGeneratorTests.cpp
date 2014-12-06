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

#include "../ArgsRefsStructMembersGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ArgsRefsStructMembersGeneratorTestsFixture {

  ArgsRefsStructMembersGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateDefinitions() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsRefsStructMembersGenerator gen(parent, &tstruct);
    gen.generateDefinitions();
    return stream.str();
  }

  std::string generateBodyDeclarations() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsRefsStructMembersGenerator gen(parent, &tstruct);
    gen.generateBodyDeclarations();
    return stream.str();
  }

  std::string generateFields() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ArgsRefsStructMembersGenerator gen(parent, &tstruct);
    gen.generateFields();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(ArgsRefsStructMembersGeneratorTests,
                         ArgsRefsStructMembersGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_generates_only_protocol_write_in_body) {
  BOOST_CHECK_EQUAL(generateBodyDeclarations(),
                    "std::uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;\n");
}

BOOST_AUTO_TEST_CASE(struct_generates_definitions_for_all_declared_methods) {
  BOOST_CHECK_EQUAL(
      generateDefinitions(),
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

BOOST_AUTO_TEST_CASE(fields_are_generated_using_references_when_needed) {
  t_field arg1(t_base_type::type_i32, "arg1", 1);
  tstruct.append(&arg1);

  t_map mapType(t_base_type::type_i32, t_base_type::type_i32);
  t_field arg2(&mapType, "arg2", 2);
  tstruct.append(&arg2);

  BOOST_CHECK_EQUAL(generateFields(),
                    "std::int32_t arg1; // 1\n"
                    "const std::unordered_map<std::int32_t, std::int32_t>& arg2; // 2\n");
}

BOOST_AUTO_TEST_SUITE_END()
