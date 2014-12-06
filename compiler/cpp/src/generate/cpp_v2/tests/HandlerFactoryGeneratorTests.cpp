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

#include "../HandlerFactoryGenerator.h"

#include <parse/t_service.h>
#include <parse/t_program.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct HandlerFactoryGeneratorTestsFixture {

  HandlerFactoryGeneratorTestsFixture() : program("/path"), service(&program) {
    service.set_name("ServiceName");
  }

  std::string generateHeader() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    HandlerFactoryGenerator gen(parent, &service);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateContents() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    HandlerFactoryGenerator gen(parent, &service);
    gen.generateContents();
    return stream.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(HandlerFactoryGeneratorTests, HandlerFactoryGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(basic_service_includes_service_iface_and_fwd_decl_apache) {
  BOOST_CHECK_EQUAL(generateHeader(),
                    "#include \"ServiceNameIface.h\"\n"
                    "\n"
                    "namespace apache { namespace thrift { struct TConnectionInfo; }}\n");
}

BOOST_AUTO_TEST_CASE(basic_service_results_in_complete_class) {
  BOOST_CHECK_EQUAL(
      generateContents(),
      "class ServiceNameHandlerFactory {\n"
      "public:\n"
      "  using Handler = ServiceNameIface;\n"
      "\n"
      "  virtual ~ServiceNameHandlerFactory() {}\n"
      "\n"
      "  virtual Handler* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;\n"
      "  virtual void releaseHandler(Handler* handler) = 0;\n"
      "};\n");
}

BOOST_AUTO_TEST_CASE(extended_service_includes_base_service_handler_factory) {
  t_service parent(&program);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(generateHeader(),
                    "#include \"ParentServiceHandlerFactory.h\"\n"
                    "#include \"ServiceNameIface.h\"\n"
                    "\n"
                    "namespace apache { namespace thrift { struct TConnectionInfo; }}\n");
}

BOOST_AUTO_TEST_CASE(extended_service_results_in_extended_class) {
  t_service parent(&program);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(
      generateContents(),
      "class ServiceNameHandlerFactory : public virtual ParentServiceHandlerFactory {\n"
      "public:\n"
      "  using Handler = ServiceNameIface;\n"
      "\n"
      "  virtual ~ServiceNameHandlerFactory() {}\n"
      "\n"
      "  virtual Handler* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;\n"
      "  virtual void releaseHandler(Handler* handler) = 0;\n"
      "};\n");
}

BOOST_AUTO_TEST_CASE(extended_base_from_different_namespace_is_prefixed) {
  t_program otherProgram("/path2");
  otherProgram.set_namespace("cpp", "other.ns");
  t_service parent(&otherProgram);
  parent.set_name("ParentService");
  service.set_extends(&parent);

  BOOST_CHECK_EQUAL(
      generateContents(),
      "class ServiceNameHandlerFactory : public virtual ::other::ns::ParentServiceHandlerFactory "
      "{\n"
      "public:\n"
      "  using Handler = ServiceNameIface;\n"
      "\n"
      "  virtual ~ServiceNameHandlerFactory() {}\n"
      "\n"
      "  virtual Handler* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;\n"
      "  virtual void releaseHandler(Handler* handler) = 0;\n"
      "};\n");
}

BOOST_AUTO_TEST_SUITE_END()
