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

#include "../StructReadGenerator.h"
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

struct StructReadGeneratorTestsFixture {

  StructReadGeneratorTestsFixture() : tstruct(nullptr) { resetUniqueNameGenerator(); }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructReadGenerator gen(parent, &tstruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructReadGenerator gen(parent, &tstruct);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateRead(const t_field& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructReadGenerator gen(parent, &tstruct);
    gen.generateFieldRead(&f);
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(StructReadGeneratorTests, StructReadGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(proper_declaration_is_generated) {
  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "std::uint32_t read(::apache::thrift::protocol::TProtocol* iprot)");
}

BOOST_AUTO_TEST_CASE(empty_struct_generates_basic_read) {
  BOOST_CHECK_EQUAL(generateBody(),
                    "std::uint32_t xfer = 0;\n"
                    "std::string fieldName;\n"
                    "::apache::thrift::protocol::TType fieldType;\n"
                    "std::int16_t fieldId;\n"
                    "\n"
                    "xfer += iprot->readStructBegin(fieldName);\n"
                    "\n"
                    "while (true) {\n"
                    "  xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);\n"
                    "  if (fieldType == ::apache::thrift::protocol::T_STOP)\n"
                    "    break;\n"
                    "  xfer += iprot->skip(fieldType);\n"
                    "  xfer += iprot->readFieldEnd();\n"
                    "}\n"
                    "\n"
                    "xfer += iprot->readStructEnd();\n"
                    "\n"
                    "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_required_field_generates_switch_and_data_check) {
  t_field f1(t_base_type::type_byte, "f1", 10);
  f1.set_req(t_field::T_REQUIRED);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generateBody(),
                    "std::uint32_t xfer = 0;\n"
                    "std::string fieldName;\n"
                    "::apache::thrift::protocol::TType fieldType;\n"
                    "std::int16_t fieldId;\n"
                    "\n"
                    "bool f1WasRead = false;\n"
                    "\n"
                    "clear();\n"
                    "\n"
                    "xfer += iprot->readStructBegin(fieldName);\n"
                    "\n"
                    "while (true) {\n"
                    "  xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);\n"
                    "  if (fieldType == ::apache::thrift::protocol::T_STOP)\n"
                    "    break;\n"
                    "\n"
                    "  switch (fieldId) {\n"
                    "    case 10:\n"
                    "      if (fieldType == ::apache::thrift::protocol::T_BYTE) {\n"
                    "        xfer += iprot->readByte(this->f1);\n"
                    "        f1WasRead = true;\n"
                    "      } else {\n"
                    "        xfer += iprot->skip(fieldType);\n"
                    "      }\n"
                    "      break;\n"
                    "    default:\n"
                    "      xfer += iprot->skip(fieldType);\n"
                    "      break;\n"
                    "  }\n"
                    "\n"
                    "  xfer += iprot->readFieldEnd();\n"
                    "}\n"
                    "\n"
                    "xfer += iprot->readStructEnd();\n"
                    "\n"
                    "using ::apache::thrift::protocol::TProtocolException;\n"
                    "if (!f1WasRead)\n"
                    "  throw TProtocolException(TProtocolException::INVALID_DATA);\n"
                    "\n"
                    "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_multiple_field_generates_switch_and_data_check) {
  t_field f1(t_base_type::type_byte, "f1", 10);
  f1.set_req(t_field::T_REQUIRED);
  tstruct.append(&f1);
  t_field f2(t_base_type::type_i32, "f2", 12);
  f2.set_req(t_field::T_REQUIRED);
  tstruct.append(&f2);
  t_field f3(t_base_type::type_i64, "f3", 24);
  f3.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f3);
  t_field f4(t_base_type::type_i64, "f4", 34);
  f4.set_req(t_field::T_OPT_IN_REQ_OUT);
  tstruct.append(&f4);

  BOOST_CHECK_EQUAL(
      generateBody(),
      "std::uint32_t xfer = 0;\n"
      "std::string fieldName;\n"
      "::apache::thrift::protocol::TType fieldType;\n"
      "std::int16_t fieldId;\n"
      "\n"
      "bool f1WasRead = false;\n"
      "bool f2WasRead = false;\n"
      "\n"
      "clear();\n"
      "\n"
      "xfer += iprot->readStructBegin(fieldName);\n"
      "\n"
      "while (true) {\n"
      "  xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);\n"
      "  if (fieldType == ::apache::thrift::protocol::T_STOP)\n"
      "    break;\n"
      "\n"
      "  switch (fieldId) {\n"
      "    case 10:\n"
      "      if (fieldType == ::apache::thrift::protocol::T_BYTE) {\n"
      "        xfer += iprot->readByte(this->f1);\n"
      "        f1WasRead = true;\n"
      "      } else {\n"
      "        xfer += iprot->skip(fieldType);\n"
      "      }\n"
      "      break;\n"
      "    case 12:\n"
      "      if (fieldType == ::apache::thrift::protocol::T_I32) {\n"
      "        xfer += iprot->readI32(this->f2);\n"
      "        f2WasRead = true;\n"
      "      } else {\n"
      "        xfer += iprot->skip(fieldType);\n"
      "      }\n"
      "      break;\n"
      "    case 24:\n"
      "      if (fieldType == ::apache::thrift::protocol::T_I64) {\n"
      "        xfer += iprot->readI64(*::apache::thrift::initializedOptional(this->f3));\n"
      "      } else {\n"
      "        xfer += iprot->skip(fieldType);\n"
      "      }\n"
      "      break;\n"
      "    case 34:\n"
      "      if (fieldType == ::apache::thrift::protocol::T_I64) {\n"
      "        xfer += iprot->readI64(this->f4);\n"
      "      } else {\n"
      "        xfer += iprot->skip(fieldType);\n"
      "      }\n"
      "      break;\n"
      "    default:\n"
      "      xfer += iprot->skip(fieldType);\n"
      "      break;\n"
      "  }\n"
      "\n"
      "  xfer += iprot->readFieldEnd();\n"
      "}\n"
      "\n"
      "xfer += iprot->readStructEnd();\n"
      "\n"
      "using ::apache::thrift::protocol::TProtocolException;\n"
      "if (!f1WasRead\n"
      " || !f2WasRead)\n"
      "  throw TProtocolException(TProtocolException::INVALID_DATA);\n"
      "\n"
      "return xfer;\n");
}

BOOST_AUTO_TEST_CASE(struct_field_read_generates_properly) {
  t_field f1(&tstruct, "f1", 1);
  BOOST_CHECK_EQUAL(generateRead(f1), "xfer += this->f1.read(iprot);\n");
}

BOOST_AUTO_TEST_CASE(exception_field_read_generates_properly) {
  tstruct.set_xception(true);
  t_field f1(&tstruct, "f1", 1);
  BOOST_CHECK_EQUAL(generateRead(f1), "xfer += this->f1.read(iprot);\n");
}

BOOST_AUTO_TEST_CASE(typedef_reads_original_type) {
  t_typedef t(nullptr, t_base_type::type_i64, "TypedefName");
  t_field f(&t, "x", 1);
  BOOST_CHECK_EQUAL(generateRead(f), "xfer += iprot->readI64(this->x);\n");
}

BOOST_AUTO_TEST_CASE(enum_field_read_casts_i32) {
  t_program p("/path");
  p.set_namespace("cpp", "test_ns");
  t_enum e(&p);
  e.set_name("EnumName");
  t_field f(&e, "a", 1);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "std::int32_t val;\n"
                    "xfer += iprot->readI32(val);\n"
                    "this->a = static_cast<::test_ns::EnumName>(val);\n");
}

BOOST_AUTO_TEST_CASE(optional_base_field_read_initializes_value) {
  t_field f(t_base_type::type_i16, "x", 1);
  f.set_req(t_field::T_OPTIONAL);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "xfer += iprot->readI16(*::apache::thrift::initializedOptional(this->x));\n");
}

BOOST_AUTO_TEST_CASE(optional_struct_field_read_initializes_value) {
  t_field f1(&tstruct, "f1", 1);
  f1.set_req(t_field::T_OPTIONAL);
  BOOST_CHECK_EQUAL(generateRead(f1),
                    "xfer += (*::apache::thrift::initializedOptional(this->f1)).read(iprot);\n");
}

BOOST_AUTO_TEST_CASE(list_field_reads_all_items) {
  t_list l(t_base_type::type_i16);
  t_field f(&l, "l", 1);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "auto& list0 = this->l;\n"
                    "list0.clear();\n"
                    "std::uint32_t listSize1{};\n"
                    "::apache::thrift::protocol::TType ignoredType3{};\n"
                    "xfer += iprot->readListBegin(ignoredType3, listSize1);\n"
                    "list0.resize(listSize1);\n"
                    "for (std::uint32_t idx2 = 0; idx2 < listSize1; ++idx2) {\n"
                    "  xfer += iprot->readI16(list0[idx2]);\n"
                    "}\n"
                    "xfer += iprot->readListEnd();\n");
}

BOOST_AUTO_TEST_CASE(nested_lists_generate_nested_loops) {
  t_list l1(t_base_type::type_i16);
  t_list l2(&l1);
  t_field f(&l2, "l", 1);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "auto& list0 = this->l;\n"
                    "list0.clear();\n"
                    "std::uint32_t listSize1{};\n"
                    "::apache::thrift::protocol::TType ignoredType3{};\n"
                    "xfer += iprot->readListBegin(ignoredType3, listSize1);\n"
                    "list0.resize(listSize1);\n"
                    "for (std::uint32_t idx2 = 0; idx2 < listSize1; ++idx2) {\n"
                    "  auto& list4 = list0[idx2];\n"
                    "  list4.clear();\n"
                    "  std::uint32_t listSize5{};\n"
                    "  ::apache::thrift::protocol::TType ignoredType7{};\n"
                    "  xfer += iprot->readListBegin(ignoredType7, listSize5);\n"
                    "  list4.resize(listSize5);\n"
                    "  for (std::uint32_t idx6 = 0; idx6 < listSize5; ++idx6) {\n"
                    "    xfer += iprot->readI16(list4[idx6]);\n"
                    "  }\n"
                    "  xfer += iprot->readListEnd();\n"
                    "}\n"
                    "xfer += iprot->readListEnd();\n");
}

BOOST_AUTO_TEST_CASE(set_field_read_and_inserts) {
  t_set s(t_base_type::type_i16);
  t_field f(&s, "s", 1);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "auto& set0 = this->s;\n"
                    "set0.clear();\n"
                    "std::uint32_t setSize1{};\n"
                    "::apache::thrift::protocol::TType ignoredType3{};\n"
                    "xfer += iprot->readSetBegin(ignoredType3, setSize1);\n"
                    "for (std::uint32_t idx2 = 0; idx2 < setSize1; ++idx2) {\n"
                    "  std::int16_t key4{};\n"
                    "  xfer += iprot->readI16(key4);\n"
                    "  set0.insert(std::move(key4));\n"
                    "}\n"
                    "xfer += iprot->readSetEnd();\n");
}

BOOST_AUTO_TEST_CASE(map_field_read_and_inserts) {
  t_map m(t_base_type::type_i16, t_base_type::type_i32);
  t_field f(&m, "m", 1);

  BOOST_CHECK_EQUAL(generateRead(f),
                    "auto& map0 = this->m;\n"
                    "map0.clear();\n"
                    "std::uint32_t mapSize1{};\n"
                    "::apache::thrift::protocol::TType ignoredType3{};\n"
                    "::apache::thrift::protocol::TType ignoredType4{};\n"
                    "xfer += iprot->readMapBegin(ignoredType3, ignoredType4, mapSize1);\n"
                    "for (std::uint32_t idx2 = 0; idx2 < mapSize1; ++idx2) {\n"
                    "  std::int16_t key5{};\n"
                    "  xfer += iprot->readI16(key5);\n"
                    "  auto& val6 = map0[key5];\n"
                    "  xfer += iprot->readI32(val6);\n"
                    "}\n"
                    "xfer += iprot->readMapEnd();\n");
}

BOOST_AUTO_TEST_CASE(struct_with_special_field_for_result_generates_different_data_check) {
  t_field f1(t_base_type::type_byte, "success", 0);
  f1.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(
      generateBody(),
      "std::uint32_t xfer = 0;\n"
      "std::string fieldName;\n"
      "::apache::thrift::protocol::TType fieldType;\n"
      "std::int16_t fieldId;\n"
      "\n"
      "bool successWasRead = false;\n"
      "\n"
      "xfer += iprot->readStructBegin(fieldName);\n"
      "\n"
      "while (true) {\n"
      "  xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);\n"
      "  if (fieldType == ::apache::thrift::protocol::T_STOP)\n"
      "    break;\n"
      "\n"
      "  switch (fieldId) {\n"
      "    case 0:\n"
      "      if (fieldType == ::apache::thrift::protocol::T_BYTE) {\n"
      "        xfer += iprot->readByte(*::apache::thrift::initializedOptional(this->success));\n"
      "        successWasRead = true;\n"
      "      } else {\n"
      "        xfer += iprot->skip(fieldType);\n"
      "      }\n"
      "      break;\n"
      "    default:\n"
      "      xfer += iprot->skip(fieldType);\n"
      "      break;\n"
      "  }\n"
      "\n"
      "  xfer += iprot->readFieldEnd();\n"
      "}\n"
      "\n"
      "xfer += iprot->readStructEnd();\n"
      "\n"
      "if (!successWasRead)\n"
      "  this->success = boost::none;\n"
      "\n"
      "return xfer;\n");
}

BOOST_AUTO_TEST_SUITE_END()
