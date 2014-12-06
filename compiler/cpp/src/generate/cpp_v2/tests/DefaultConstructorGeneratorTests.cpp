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

#include "../DefaultConstructorGenerator.h"
#include "../Utils.h"

#include <parse/t_base_type.h>
#include <parse/t_const_value.h>
#include <parse/t_enum.h>
#include <parse/t_enum_value.h>
#include <parse/t_field.h>
#include <parse/t_list.h>
#include <parse/t_map.h>
#include <parse/t_program.h>
#include <parse/t_set.h>
#include <parse/t_struct.h>
#include <parse/t_typedef.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct DefaultConstructorGeneratorTestsFixture {

  DefaultConstructorGeneratorTestsFixture()
    : structType(nullptr),
      i32Field(t_base_type::type_i32, "i", 1),
      stringField(t_base_type::type_i32, "str", 2),
      structField(&structType, "s", 1),
      tStruct(nullptr) {
    resetUniqueNameGenerator();

    tStruct.set_name("StructName");

    structType.set_name("InnerStruct");
    structType.append(&i32Field);
    structType.append(&stringField);
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    DefaultConstructorGenerator gen(parent, &tStruct);
    gen.generateDeclaration();
    return stream.str();
  }

  std::string generateFieldValue(const t_field& f) {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    DefaultConstructorGenerator gen(parent, &tStruct);
    gen.generateFieldValue(&f);
    return stream.str();
  }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    DefaultConstructorGenerator gen(parent, &tStruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  struct I32FieldConst {
    I32FieldConst() : fieldName("i"), fieldValue(123) {}

    void addToMap(t_const_value& m) {
      m.set_map();
      m.add_map(&fieldName, &fieldValue);
    }

    void addToList(t_const_value& l) {
      l.set_list();
      l.add_list(&fieldValue);
    }

    t_const_value fieldName;
    t_const_value fieldValue;
  };

  void addField(t_field& f) { tStruct.append(&f); }

  t_struct structType;
  t_field i32Field;
  t_field stringField;
  t_field structField;

private:
  t_struct tStruct;
};

BOOST_FIXTURE_TEST_SUITE(DefaultConstructorGeneratorTests, DefaultConstructorGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_name_is_used_in_declaration) {
  BOOST_CHECK_EQUAL(generateDeclaration(), "StructName()");
}

BOOST_AUTO_TEST_CASE(empty_struct_generates_empty_body) {
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(base_type_field_initializes_in_init_list) {
  t_const_value c(12);
  i32Field.set_value(&c);
  addField(i32Field);

  BOOST_CHECK_EQUAL(generateFieldValue(i32Field), "12");
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(string_field_initializes_in_init_list_in_quotes) {
  t_const_value c("12");
  stringField.set_value(&c);
  addField(stringField);

  BOOST_CHECK_EQUAL(generateFieldValue(stringField), "\"12\"");
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(typedefed_base_type_field_initializes_in_init_list) {
  t_typedef t2(nullptr, t_base_type::type_string, "TypeName");
  t_const_value c("12");
  t_field f(&t2, "f", 1);
  f.set_value(&c);
  addField(f);

  BOOST_CHECK_EQUAL(generateFieldValue(f), "\"12\"");
}

BOOST_AUTO_TEST_CASE(enum_initializes_in_init_list_by_identifier) {
  t_program p("/path");
  p.set_namespace("cpp", "test");

  t_enum_value ev("Val1", 1);
  t_enum e(&p);
  e.set_name("EnumName");
  e.append(&ev);

  t_const_value val;
  val.set_identifier("EnumName.Val1");

  t_field f(&e, "f", 1);
  f.set_value(&val);
  addField(f);

  BOOST_CHECK_EQUAL(generateFieldValue(f), "::test::EnumName::Val1");
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(enum_initializes_in_init_list_by_integer) {
  t_program p("/path");
  p.set_namespace("cpp", "test");

  t_enum_value ev("Val1", 1);
  t_enum e(&p);
  e.set_name("EnumName");
  e.append(&ev);

  t_const_value val(1);

  t_field f(&e, "f", 1);
  f.set_value(&val);
  addField(f);

  BOOST_CHECK_EQUAL(generateFieldValue(f), "static_cast<::test::EnumName>(1)");
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(struct_field_initializes_members_in_body) {
  t_const_value val;
  I32FieldConst c;
  c.addToMap(val);

  structField.set_value(&val);
  addField(structField);

  BOOST_CHECK(generateFieldValue(structField).empty());
  BOOST_CHECK_EQUAL(generateBody(), "s.i = 123;\n");
}

BOOST_AUTO_TEST_CASE(optional_struct_field_initializes_members_in_body) {
  t_const_value val;
  I32FieldConst c;
  c.addToMap(val);

  structField.set_value(&val);
  structField.set_req(t_field::T_OPTIONAL);
  addField(structField);

  BOOST_CHECK(generateFieldValue(structField).empty());
  BOOST_CHECK_EQUAL(generateBody(),
                    "s = InnerStruct{};\n"
                    "s->i = 123;\n");
}

BOOST_AUTO_TEST_CASE(nested_struct_field_initializes_members_in_body) {
  t_struct innerStruct(nullptr);
  innerStruct.append(&structField);

  const std::string innerStructFieldName = "f";

  t_const_value innerValue;
  I32FieldConst c;
  c.addToMap(innerValue);

  t_const_value innerKey("s");

  t_const_value value;
  value.set_map();
  value.add_map(&innerKey, &innerValue);

  t_field f(&innerStruct, "f", 1);
  f.set_value(&value);
  addField(f);

  BOOST_CHECK(generateFieldValue(f).empty());
  BOOST_CHECK_EQUAL(generateBody(), "f.s.i = 123;\n");
}

BOOST_AUTO_TEST_CASE(list_field_initializes_in_body) {
  t_list l(t_base_type::type_i32);
  t_field f(&l, "l", 12);

  t_const_value v;
  I32FieldConst c;
  c.addToList(v);
  c.addToList(v);
  f.set_value(&v);

  addField(f);

  BOOST_CHECK(generateFieldValue(f).empty());
  BOOST_CHECK_EQUAL(generateBody(),
                    "l.push_back(123);\n"
                    "l.push_back(123);\n");
}

BOOST_AUTO_TEST_CASE(optional_list_of_structs_field_initializes_in_body) {
  t_list l(&structType);
  t_field f(&l, "l", 12);

  t_const_value inner;
  I32FieldConst c;
  c.addToMap(inner);

  t_const_value v;
  v.set_list();
  v.add_list(&inner);
  v.add_list(&inner);
  f.set_req(t_field::T_OPTIONAL);
  f.set_value(&v);

  addField(f);

  BOOST_CHECK(generateFieldValue(f).empty());
  BOOST_CHECK_EQUAL(generateBody(),
                    "l = std::vector<InnerStruct>{};\n"
                    "l->emplace_back();\n"
                    "l->back().i = 123;\n"
                    "l->emplace_back();\n"
                    "l->back().i = 123;\n");
}

BOOST_AUTO_TEST_CASE(struct_with_list_field_initializes_in_body_properly) {
  t_list l(t_base_type::type_i32);
  t_field lf(&l, "l", 12);

  structType.append(&lf);

  t_const_value listValue;
  I32FieldConst c;
  c.addToList(listValue);

  t_const_value listKey("l");

  t_const_value value;
  value.set_map();
  value.add_map(&listKey, &listValue);
  structField.set_value(&value);

  addField(structField);

  BOOST_CHECK_EQUAL(generateBody(), "s.l.push_back(123);\n");
}

BOOST_AUTO_TEST_CASE(struct_with_optional_list_field_initializes_in_body_properly) {
  t_list l(t_base_type::type_i32);
  t_field lf(&l, "l", 12);
  lf.set_req(t_field::T_OPTIONAL);

  structType.append(&lf);

  t_const_value listValue;
  I32FieldConst c;
  c.addToList(listValue);

  t_const_value listKey("l");

  t_const_value value;

  value.set_map();
  value.add_map(&listKey, &listValue);
  structField.set_value(&value);

  addField(structField);

  BOOST_CHECK_EQUAL(generateBody(),
                    "s.l = std::vector<std::int32_t>{};\n"
                    "s.l->push_back(123);\n");
}

BOOST_AUTO_TEST_CASE(nested_list_initializes_in_body_properly) {
  t_list inner(t_base_type::type_i32);
  t_list l(&inner);

  t_const_value innerV;
  I32FieldConst c;
  c.addToList(innerV);

  t_const_value val;
  val.set_list();
  val.add_list(&innerV);

  t_field f(&l, "l", 2);
  f.set_value(&val);
  addField(f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "l.emplace_back();\n"
                    "l.back().push_back(123);\n");
}

BOOST_AUTO_TEST_CASE(set_field_initializes_properly) {
  t_set s(t_base_type::type_i32);
  t_field f(&s, "s", 12);

  t_const_value val;
  I32FieldConst c;
  c.addToList(val);
  f.set_value(&val);

  addField(f);

  BOOST_CHECK_EQUAL(generateBody(), "s.insert(123);\n");
}

BOOST_AUTO_TEST_CASE(set_field_with_complex_items_initializes_properly) {
  t_set s(&structType);
  t_field f(&s, "s", 12);

  t_const_value innerV;
  I32FieldConst c;
  c.addToMap(innerV);

  t_const_value val;
  val.set_list();
  val.add_list(&innerV);
  f.set_value(&val);

  addField(f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "InnerStruct tmp0;\n"
                    "tmp0.i = 123;\n"
                    "s.insert(std::move(tmp0));\n");
}

BOOST_AUTO_TEST_CASE(map_field_initializes_properly) {
  t_map m(t_base_type::type_string, t_base_type::type_i32);
  t_field f(&m, "m", 1);

  t_const_value v;
  I32FieldConst c;
  c.addToMap(v);

  f.set_value(&v);

  addField(f);

  BOOST_CHECK_EQUAL(generateBody(), "m.emplace(\"i\", 123);\n");
}

BOOST_AUTO_TEST_CASE(map_with_complex_value_initializes_properly) {
  t_map m(t_base_type::type_i32, &structType);
  t_field f(&m, "m", 1);

  t_const_value innerV;
  I32FieldConst c;
  c.addToMap(innerV);

  t_const_value innerK(567);

  t_const_value v;
  v.set_map();
  v.add_map(&innerK, &innerV);

  f.set_value(&v);

  addField(f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "InnerStruct tmpVal1;\n"
                    "tmpVal1.i = 123;\n"
                    "m.emplace(567, std::move(tmpVal1));\n");
}

BOOST_AUTO_TEST_CASE(map_with_complex_key_initializes_properly) {
  t_map m(&structType, t_base_type::type_i32);
  t_field f(&m, "m", 1);

  t_const_value innerK;
  I32FieldConst c;
  c.addToMap(innerK);

  t_const_value innerV(567);

  t_const_value v;
  v.set_map();
  v.add_map(&innerK, &innerV);

  f.set_value(&v);

  addField(f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "InnerStruct tmpKey0;\n"
                    "tmpKey0.i = 123;\n"
                    "m.emplace(std::move(tmpKey0), 567);\n");
}

// TODO constants assignment

BOOST_AUTO_TEST_SUITE_END()
