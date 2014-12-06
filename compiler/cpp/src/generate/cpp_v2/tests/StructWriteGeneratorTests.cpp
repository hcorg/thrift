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

#include "../StructWriteGenerator.h"
#include "../Utils.h"

#include <parse/t_base_type.h>
#include <parse/t_enum.h>
#include <parse/t_list.h>
#include <parse/t_map.h>
#include <parse/t_program.h>
#include <parse/t_set.h>
#include <parse/t_struct.h>
#include <parse/t_typedef.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct StructWriteGeneratorTestsFixture {

  StructWriteGeneratorTestsFixture() : tstruct(nullptr) {
    tstruct.set_name("StructName");
    resetUniqueNameGenerator();
  }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructWriteGenerator gen(parent, &tstruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructWriteGenerator gen(parent, &tstruct);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateWrite(const t_field& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructWriteGenerator gen(parent, &tstruct);
    gen.generateFieldWrite(&f);
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(StructWriteGeneratorTests, StructWriteGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(proper_declaration_is_generated) {
  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "std::uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const");
}

BOOST_AUTO_TEST_CASE(empty_struct_generates_basic_write) {
  BOOST_CHECK_EQUAL(generateBody(),
                    "std::uint32_t xfer = 0;\n"
                    "oprot->incrementRecursionDepth();\n"
                    "xfer += oprot->writeStructBegin(\"StructName\");\n"
                    "\n"
                    "xfer += oprot->writeFieldStop();\n"
                    "xfer += oprot->writeStructEnd();\n"
                    "oprot->decrementRecursionDepth();\n"
                    "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_required_field_generates_write_for_it) {
  t_field f1(t_base_type::type_byte, "f1", 10);
  f1.set_req(t_field::T_REQUIRED);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(
      generateBody(),
      "std::uint32_t xfer = 0;\n"
      "oprot->incrementRecursionDepth();\n"
      "xfer += oprot->writeStructBegin(\"StructName\");\n"
      "\n"
      "xfer += oprot->writeFieldBegin(\"f1\", ::apache::thrift::protocol::T_BYTE, 10);\n"
      "xfer += oprot->writeByte(this->f1);\n"
      "xfer += oprot->writeFieldEnd();\n"
      "\n"
      "xfer += oprot->writeFieldStop();\n"
      "xfer += oprot->writeStructEnd();\n"
      "oprot->decrementRecursionDepth();\n"
      "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_normal_field_generates_write_for_it) {
  t_field f1(t_base_type::type_byte, "f1", 10);
  f1.set_req(t_field::T_OPT_IN_REQ_OUT);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(
      generateBody(),
      "std::uint32_t xfer = 0;\n"
      "oprot->incrementRecursionDepth();\n"
      "xfer += oprot->writeStructBegin(\"StructName\");\n"
      "\n"
      "xfer += oprot->writeFieldBegin(\"f1\", ::apache::thrift::protocol::T_BYTE, 10);\n"
      "xfer += oprot->writeByte(this->f1);\n"
      "xfer += oprot->writeFieldEnd();\n"
      "\n"
      "xfer += oprot->writeFieldStop();\n"
      "xfer += oprot->writeStructEnd();\n"
      "oprot->decrementRecursionDepth();\n"
      "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_optional_field_generates_optional_write_for_it) {
  t_field f1(t_base_type::type_byte, "f1", 10);
  f1.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(
      generateBody(),
      "std::uint32_t xfer = 0;\n"
      "oprot->incrementRecursionDepth();\n"
      "xfer += oprot->writeStructBegin(\"StructName\");\n"
      "\n"
      "if (this->f1) {\n"
      "  xfer += oprot->writeFieldBegin(\"f1\", ::apache::thrift::protocol::T_BYTE, 10);\n"
      "  xfer += oprot->writeByte(*(this->f1));\n"
      "  xfer += oprot->writeFieldEnd();\n"
      "}\n"
      "\n"
      "xfer += oprot->writeFieldStop();\n"
      "xfer += oprot->writeStructEnd();\n"
      "oprot->decrementRecursionDepth();\n"
      "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(typedef_writes_original_type) {
  t_typedef t(nullptr, t_base_type::type_i64, "TypedefName");
  t_field f(&t, "x", 1);
  BOOST_CHECK_EQUAL(generateWrite(f), "xfer += oprot->writeI64(this->x);\n");
}

BOOST_AUTO_TEST_CASE(enum_field_writes_casts_i32) {
  t_program p("/path");
  p.set_namespace("cpp", "test_ns");
  t_enum e(&p);
  e.set_name("EnumName");
  t_field f(&e, "a", 1);

  BOOST_CHECK_EQUAL(generateWrite(f),
                    "xfer += oprot->writeI32(static_cast<std::int32_t>(this->a));\n");
}

BOOST_AUTO_TEST_CASE(struct_field_write_generates_properly) {
  t_field f1(&tstruct, "f1", 1);
  BOOST_CHECK_EQUAL(generateWrite(f1), "xfer += this->f1.write(oprot);\n");
}

BOOST_AUTO_TEST_CASE(exception_field_write_generates_properly) {
  tstruct.set_xception(true);
  t_field f1(&tstruct, "f1", 1);
  BOOST_CHECK_EQUAL(generateWrite(f1), "xfer += this->f1.write(oprot);\n");
}

BOOST_AUTO_TEST_CASE(optional_struct_field_writes_properly) {
  t_field f1(&tstruct, "f1", 1);
  f1.set_req(t_field::T_OPTIONAL);
  BOOST_CHECK_EQUAL(generateWrite(f1), "xfer += (*(this->f1)).write(oprot);\n");
}

BOOST_AUTO_TEST_CASE(list_field_writes_all_items) {
  t_list l(t_base_type::type_i16);
  t_field f(&l, "l", 1);

  BOOST_CHECK_EQUAL(generateWrite(f),
                    "xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I16, "
                    "static_cast<std::uint32_t>(this->l.size()));\n"
                    "for (const auto& listItem0 : this->l) {\n"
                    "  xfer += oprot->writeI16(listItem0);\n"
                    "}\n"
                    "xfer += oprot->writeListEnd();\n");
}

BOOST_AUTO_TEST_CASE(nested_lists_generate_nested_loops) {
  t_list l1(t_base_type::type_i16);
  t_list l2(&l1);
  t_field f(&l2, "l", 1);

  BOOST_CHECK_EQUAL(generateWrite(f),
                    "xfer += oprot->writeListBegin(::apache::thrift::protocol::T_LIST, "
                    "static_cast<std::uint32_t>(this->l.size()));\n"
                    "for (const auto& listItem0 : this->l) {\n"
                    "  xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I16, "
                    "static_cast<std::uint32_t>(listItem0.size()));\n"
                    "  for (const auto& listItem1 : listItem0) {\n"
                    "    xfer += oprot->writeI16(listItem1);\n"
                    "  }\n"
                    "  xfer += oprot->writeListEnd();\n"
                    "}\n"
                    "xfer += oprot->writeListEnd();\n");
}

BOOST_AUTO_TEST_CASE(set_field_write_all_items) {
  t_set s(t_base_type::type_i16);
  t_field f(&s, "s", 1);

  BOOST_CHECK_EQUAL(generateWrite(f),
                    "xfer += oprot->writeSetBegin(::apache::thrift::protocol::T_I16, "
                    "static_cast<std::uint32_t>(this->s.size()));\n"
                    "for (const auto& setItem0 : this->s) {\n"
                    "  xfer += oprot->writeI16(setItem0);\n"
                    "}\n"
                    "xfer += oprot->writeSetEnd();\n");
}

BOOST_AUTO_TEST_CASE(map_field_write_al_items) {
  t_map m(t_base_type::type_i16, t_base_type::type_i32);
  t_field f(&m, "m", 1);

  BOOST_CHECK_EQUAL(generateWrite(f),
                    "xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_I16, "
                    "::apache::thrift::protocol::T_I32, "
                    "static_cast<std::uint32_t>(this->m.size()));\n"
                    "for (const auto& mapItem0 : this->m) {\n"
                    "  xfer += oprot->writeI16(mapItem0.first);\n"
                    "  xfer += oprot->writeI32(mapItem0.second);\n"
                    "}\n"
                    "xfer += oprot->writeMapEnd();\n");
}

BOOST_AUTO_TEST_SUITE_END()
