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

#include "../StructClearGenerator.h"
#include "../Utils.h"

#include <parse/t_base_type.h>
#include <parse/t_const.h>
#include <parse/t_field.h>
#include <parse/t_list.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct StructClearGeneratorTestsFixture {

  StructClearGeneratorTestsFixture() : tstruct(nullptr) { resetUniqueNameGenerator(); }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructClearGenerator gen(parent, &tstruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructClearGenerator gen(parent, &tstruct);
    gen.generateDeclaration();
    return stream.str();
  }

  struct I32FieldConst {
    I32FieldConst() : fieldName("i"), fieldValue(123) {}

    void addToList(t_const_value& l) {
      l.set_list();
      l.add_list(&fieldValue);
    }

    t_const_value fieldName;
    t_const_value fieldValue;
  };

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(StructClearGeneratorTests, StructClearGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(simple_declaration_is_generated) {
  BOOST_CHECK_EQUAL(generateDeclaration(), "void clear()");
}

BOOST_AUTO_TEST_CASE(no_fields_generates_empty_body) {
  BOOST_CHECK(generateBody().empty());
}

BOOST_AUTO_TEST_CASE(optional_field_gets_reset) {
  t_field f1(t_base_type::type_i32, "f1", 1);
  f1.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generateBody(),
                    "f1 = boost::none;\n"
                    "\n");
}

BOOST_AUTO_TEST_CASE(nonoptional_field_without_default_value_is_set_type_default) {
  t_field f1(t_base_type::type_i32, "f1", 1);
  f1.set_req(t_field::T_REQUIRED);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generateBody(), "f1 = {};\n");
}

BOOST_AUTO_TEST_CASE(nonoptional_field_with_default_value_is_set_to_it) {
  t_field f1(t_base_type::type_i32, "f1", 1);
  f1.set_req(t_field::T_REQUIRED);
  t_const_value v(42);
  f1.set_value(&v);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generateBody(), "f1 = 42;\n");
}

BOOST_AUTO_TEST_CASE(string_field_without_default_value_is_cleared) {
  t_field f1(t_base_type::type_string, "f1", 1);
  f1.set_req(t_field::T_REQUIRED);
  tstruct.append(&f1);

  BOOST_CHECK_EQUAL(generateBody(), "f1.clear();\n");
}

BOOST_AUTO_TEST_CASE(multiple_fields_generate_properly) {
  t_field f1(t_base_type::type_i32, "f1", 1);
  f1.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f1);

  t_field f2(t_base_type::type_string, "f2", 2);
  f2.set_req(t_field::T_REQUIRED);
  tstruct.append(&f2);

  t_field f3(t_base_type::type_string, "f3", 3);
  f3.set_req(t_field::T_REQUIRED);
  t_const_value v("42");
  f3.set_value(&v);
  tstruct.append(&f3);

  BOOST_CHECK_EQUAL(generateBody(),
                    "f1 = boost::none;\n"
                    "\n"
                    "f2.clear();\n"
                    "f3 = \"42\";\n");
}

BOOST_AUTO_TEST_CASE(list_field_is_cleared) {
  t_list l(t_base_type::type_i32);
  t_field f(&l, "l", 12);

  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(), "l.clear();\n");
}

BOOST_AUTO_TEST_CASE(list_field_with_default_value_resets_to_it) {
  t_list l(t_base_type::type_i32);
  t_field f(&l, "l", 12);

  t_const_value v;
  I32FieldConst c;
  c.addToList(v);
  c.addToList(v);
  f.set_value(&v);

  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "l.clear();\n"
                    "l.push_back(123);\n"
                    "l.push_back(123);\n");
}

BOOST_AUTO_TEST_CASE(optional_list_field_with_default_value_is_reset) {
  t_list l(t_base_type::type_i32);
  t_field f(&l, "l", 12);
  f.set_req(t_field::T_OPTIONAL);

  t_const_value v;
  I32FieldConst c;
  c.addToList(v);
  c.addToList(v);
  f.set_value(&v);

  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "l = boost::none;\n"
                    "\n");
}

BOOST_AUTO_TEST_SUITE_END()
