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

#include "../IfaceGenerator.h"

#include <parse/t_base_type.h>
#include <parse/t_function.h>
#include <parse/t_program.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct IfaceGeneratorTestsFixture {

  IfaceGeneratorTestsFixture() : program("/path"), service(&program) {
    service.set_name("ServiceName");
  }

  std::string generateHeader() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    IfaceGenerator gen(parent, &service);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateContents() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    IfaceGenerator gen(parent, &service);
    gen.generateContents();
    return stream.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(IfaceGeneratorTests, IfaceGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(empty_service_does_not_generate_includes) {
  BOOST_CHECK(generateHeader().empty());
}

BOOST_AUTO_TEST_CASE(return_type_of_function_adds_to_includes) {
  t_struct args(nullptr);
  t_function f(t_base_type::type_string, "fun", &args);
  service.add_function(&f);

  BOOST_CHECK_EQUAL(generateHeader(), "#include <string>\n");
}

BOOST_AUTO_TEST_CASE(args_types_add_to_includes) {
  t_struct args(nullptr);
  t_field arg(t_base_type::type_string, "arg", 1);
  args.append(&arg);
  t_function f(t_base_type::type_void, "fun", &args);
  service.add_function(&f);

  BOOST_CHECK_EQUAL(generateHeader(), "#include <string>\n");
}

BOOST_AUTO_TEST_CASE(exception_types_add_to_includes) {
  t_struct args(nullptr);
  t_struct except(nullptr);
  t_field e(t_base_type::type_string, "e", 1);
  except.append(&e);
  t_function f(t_base_type::type_void, "fun", &args, &except);
  service.add_function(&f);

  BOOST_CHECK_EQUAL(generateHeader(), "#include <string>\n");
}

BOOST_AUTO_TEST_CASE(extends_adds_to_includes) {
  t_service parent(&program);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(generateHeader(), "#include \"ParentServiceIface.h\"\n");
}

BOOST_AUTO_TEST_CASE(empty_service_generates_basic_iface) {
  BOOST_CHECK_EQUAL(generateContents(),
                    "class ServiceNameIface {\n"
                    "public:\n"
                    "  virtual ~ServiceNameIface() {}\n"
                    "\n"
                    "};");
}

BOOST_AUTO_TEST_CASE(single_function_adds_to_iface) {
  t_struct args(nullptr);
  t_function f(t_base_type::type_string, "fun", &args);
  service.add_function(&f);

  BOOST_CHECK_EQUAL(generateContents(),
                    "class ServiceNameIface {\n"
                    "public:\n"
                    "  virtual ~ServiceNameIface() {}\n"
                    "\n"
                    "  virtual std::string fun() = 0;\n"
                    "};");
}

BOOST_AUTO_TEST_CASE(multiple_functions_adds_to_iface) {
  t_struct args(nullptr);
  t_function f1(t_base_type::type_void, "fun1", &args);
  service.add_function(&f1);
  t_function f2(t_base_type::type_string, "fun2", &args);
  service.add_function(&f2);

  BOOST_CHECK_EQUAL(generateContents(),
                    "class ServiceNameIface {\n"
                    "public:\n"
                    "  virtual ~ServiceNameIface() {}\n"
                    "\n"
                    "  virtual void fun1() = 0;\n"
                    "  virtual std::string fun2() = 0;\n"
                    "};");
}

BOOST_AUTO_TEST_CASE(extended_service_inherits_base_service) {
  t_service parent(&program);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(generateContents(),
                    "class ServiceNameIface : public virtual ParentServiceIface {\n"
                    "public:\n"
                    "  virtual ~ServiceNameIface() {}\n"
                    "\n"
                    "};");
}

BOOST_AUTO_TEST_CASE(extended_service_inherits_base_service_from_another_namespace) {
  t_program program2("/path2");
  program2.set_namespace("cpp", "other.name");
  t_service parent(&program2);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(generateContents(),
                    "class ServiceNameIface : public virtual ::other::name::ParentServiceIface {\n"
                    "public:\n"
                    "  virtual ~ServiceNameIface() {}\n"
                    "\n"
                    "};");
}

BOOST_AUTO_TEST_SUITE_END()
