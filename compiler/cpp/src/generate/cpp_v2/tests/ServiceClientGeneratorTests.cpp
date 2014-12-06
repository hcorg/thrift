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

#include "../ServiceClientGenerator.h"

#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include <parse/t_service.h>
#include <parse/t_program.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ServiceClientGeneratorTestsFixture {

  ServiceClientGeneratorTestsFixture() : program("/path"), service(&program) {
    service.set_name("ServiceName");
  }

  std::string generateHeaderIncludes() {
    std::ostringstream out;
    OutputGenerator parent(out);
    ServiceClientGenerator::HeaderGenerator gen(parent, &service);
    gen.generateHeader();
    return out.str();
  }

  std::string generateSourceIncludes() {
    std::ostringstream out;
    OutputGenerator parent(out);
    ServiceClientGenerator::SourceGenerator gen(parent, &service);
    gen.generateHeader();
    return out.str();
  }

  std::string generateDeclaration() {
    std::ostringstream out;
    OutputGenerator parent(out);
    ServiceClientGenerator::HeaderGenerator gen(parent, &service);
    gen.generateContents();
    return out.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(ServiceClientGeneratorTests, ServiceClientGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(service_name_reflects_client_name) {
  BOOST_CHECK_EQUAL(ServiceClientGenerator::getFileNameBase(&service), "ServiceNameClient");
}

BOOST_AUTO_TEST_CASE(service_client_includes_service_iface_and_client_base) {
  BOOST_CHECK_EQUAL(generateHeaderIncludes(),
                    "#include <thrift/client/ClientBase.h>\n"
                    "\n"
                    "#include \"ServiceNameIface.h\"\n");
}

BOOST_AUTO_TEST_CASE(extended_service_client_includes_parent_service_client) {
  t_service parent(&program);
  parent.set_name("ParentService");
  service.set_extends(&parent);
  BOOST_CHECK_EQUAL(generateHeaderIncludes(),
                    "#include \"ParentServiceClient.h\"\n"
                    "#include \"ServiceNameIface.h\"\n");
}

BOOST_AUTO_TEST_CASE(service_client_extends_ClientBase_and_calls_it_constructors) {
  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "class ServiceNameClient\n"
                    "  : public ::apache::thrift::client::ClientBase,\n"
                    "    public virtual ServiceNameIface {\n"
                    "public:\n"
                    "  ServiceNameClient(TProtocolPtr prot)\n"
                    "    : ::apache::thrift::client::ClientBase(prot) {}\n"
                    "  ServiceNameClient(TProtocolPtr iprot, TProtocolPtr oprot)\n"
                    "    : ::apache::thrift::client::ClientBase(iprot, oprot) {}\n"
                    "\n"
                    "\n"
                    "private:\n"
                    "};\n");
}

BOOST_AUTO_TEST_CASE(extended_service_client_extends_parent_client_and_calls_it_constructors) {
  t_program parentProgram("/path2");
  parentProgram.set_namespace("cpp", "other");
  t_service parent(&parentProgram);
  parent.set_name("ParentService");
  service.set_extends(&parent);
  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "class ServiceNameClient\n"
                    "  : public ::other::ParentServiceClient,\n"
                    "    public virtual ServiceNameIface {\n"
                    "public:\n"
                    "  ServiceNameClient(TProtocolPtr prot)\n"
                    "    : ::other::ParentServiceClient(prot) {}\n"
                    "  ServiceNameClient(TProtocolPtr iprot, TProtocolPtr oprot)\n"
                    "    : ::other::ParentServiceClient(iprot, oprot) {}\n"
                    "\n"
                    "\n"
                    "private:\n"
                    "};\n");
}

BOOST_AUTO_TEST_CASE(source_file_includes_service_internal_objects) {
  BOOST_CHECK_EQUAL(generateSourceIncludes(), "#include \"ServiceNameInternalObjects.h\"\n");
}

BOOST_AUTO_TEST_SUITE_END()
