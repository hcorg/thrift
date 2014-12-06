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
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

#include <boost/test/unit_test.hpp>

#include "../Utils.h"

#include <parse/t_typedef.h>
#include <parse/t_base_type.h>
#include <parse/t_struct.h>
#include <parse/t_program.h>
#include <parse/t_map.h>
#include <parse/t_set.h>
#include <parse/t_list.h>
#include <parse/t_enum.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

BOOST_AUTO_TEST_CASE(namespaceOpen_accepts_empty_name) {
  BOOST_CHECK_EQUAL(namespaceOpen(""), "namespace  {");
}

BOOST_AUTO_TEST_CASE(namespaceOpen_accepts_standalone_namespace) {
  BOOST_CHECK_EQUAL(namespaceOpen("name"), "namespace name {");
}

BOOST_AUTO_TEST_CASE(namespaceOpen_accepts_nested_namespaces) {
  BOOST_CHECK_EQUAL(namespaceOpen("name.nested.name2"),
                    "namespace name { namespace nested { namespace name2 {");
}

BOOST_AUTO_TEST_CASE(namespace_close_accepts_empty_name) {
  BOOST_CHECK_EQUAL(namespaceClose(""), "} // namespace ");
}

BOOST_AUTO_TEST_CASE(namespaceClose_accepts_standalone_namespace) {
  BOOST_CHECK_EQUAL(namespaceClose("name"), "} // namespace name");
}

BOOST_AUTO_TEST_CASE(namespaceClose_accepts_nested_namespaces) {
  BOOST_CHECK_EQUAL(namespaceClose("name.nested.name2"), "}}} // namespace name::nested::name2");
}

BOOST_AUTO_TEST_CASE(namespaceName_accepts_empty_name) {
  BOOST_CHECK_EQUAL(namespaceName(""), "");
}

BOOST_AUTO_TEST_CASE(namespaceName_accepts_standalone_namespace) {
  BOOST_CHECK_EQUAL(namespaceName("name"), "name");
}

BOOST_AUTO_TEST_CASE(namespaceName_accepts_nested_namespace) {
  BOOST_CHECK_EQUAL(namespaceName("name.nested.name2"), "name::nested::name2");
}

BOOST_AUTO_TEST_CASE(namespacePrefix_accepts_empty_name) {
  BOOST_CHECK_EQUAL(namespacePrefix(""), "");
}

BOOST_AUTO_TEST_CASE(namespacePrefix_accepts_standalone_namespace) {
  BOOST_CHECK_EQUAL(namespacePrefix("name"), "::name::");
}

BOOST_AUTO_TEST_CASE(namespacePrefix_accepts_nested_namespace) {
  BOOST_CHECK_EQUAL(namespacePrefix("name.nested2.name2"), "::name::nested2::name2::");
}

BOOST_AUTO_TEST_CASE(getCppNamespace_returns_proper_namespace) {
  const std::string name = "some.namespace";

  t_program program("/path");
  program.set_namespace("cpp", name);

  t_struct type(&program, "StructName");

  BOOST_CHECK_EQUAL(getCppNamespace(&type), name);
}

BOOST_AUTO_TEST_CASE(getTTypeString_support_base_types) {
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_void), "::apache::thrift::protocol::T_VOID");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_bool), "::apache::thrift::protocol::T_BOOL");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_byte), "::apache::thrift::protocol::T_BYTE");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_double),
                    "::apache::thrift::protocol::T_DOUBLE");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_i16), "::apache::thrift::protocol::T_I16");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_i32), "::apache::thrift::protocol::T_I32");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_i64), "::apache::thrift::protocol::T_I64");
  BOOST_CHECK_EQUAL(getTTypeString(t_base_type::type_string),
                    "::apache::thrift::protocol::T_STRING");
}

BOOST_AUTO_TEST_CASE(getTTypeString_support_container_types) {
  t_list l(nullptr);
  BOOST_CHECK_EQUAL(getTTypeString(&l), "::apache::thrift::protocol::T_LIST");

  t_set s(nullptr);
  BOOST_CHECK_EQUAL(getTTypeString(&s), "::apache::thrift::protocol::T_SET");

  t_map m(nullptr, nullptr);
  BOOST_CHECK_EQUAL(getTTypeString(&m), "::apache::thrift::protocol::T_MAP");
}

BOOST_AUTO_TEST_CASE(getTTypeString_support_enum_type) {
  t_enum e(nullptr);
  BOOST_CHECK_EQUAL(getTTypeString(&e), "::apache::thrift::protocol::T_I32");
}

BOOST_AUTO_TEST_CASE(getTTypeString_support_structs_and_exceptions) {
  t_struct s(nullptr);
  BOOST_CHECK_EQUAL(getTTypeString(&s), "::apache::thrift::protocol::T_STRUCT");

  s.set_xception(true);
  BOOST_CHECK_EQUAL(getTTypeString(&s), "::apache::thrift::protocol::T_STRUCT");
}

BOOST_AUTO_TEST_CASE(getTTypeString_supports_typedefs) {
  t_list l(nullptr);
  t_typedef t(nullptr, &l, "Type");
  BOOST_CHECK_EQUAL(getTTypeString(&t), "::apache::thrift::protocol::T_LIST");
}

BOOST_AUTO_TEST_CASE(namespacePrefixIfDifferent_return_empty_when_equal) {
  BOOST_CHECK(namespacePrefixIfDifferent("name1", "name1").empty());
}

BOOST_AUTO_TEST_CASE(namespacePrefixIfDifferent_return_others_prefix_when_different) {
  BOOST_CHECK_EQUAL(namespacePrefixIfDifferent("name1", "name2"), "::name2::");
}

BOOST_AUTO_TEST_CASE(localInclude_returns_quoted_include) {
  BOOST_CHECK_EQUAL(localInclude("IncludeName"), "#include \"IncludeName\"");
}

BOOST_AUTO_TEST_CASE(systemInclude_returns_include_in_brackets) {
  BOOST_CHECK_EQUAL(systemInclude("IncludeName"), "#include <IncludeName>");
}
