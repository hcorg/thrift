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

#include "../ServiceInternalObjectsGenerator.h"
#include "../ServiceInternalStructListGenerator.h"

#include <parse/t_program.h>
#include <parse/t_service.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct ServiceInternalObjectsGeneratorTestsFixture {

  ServiceInternalObjectsGeneratorTestsFixture() : program("/path"), service(&program) {
    service.set_name("ServiceName");
  }

  std::string generateHeaderIncludes() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ServiceInternalObjectsGenerator::HeaderGenerator gen(parent, &service);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateSourceIncludes() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    ServiceInternalObjectsGenerator::SourceGenerator gen(parent, &service);
    gen.generateHeader();
    return stream.str();
  }

  t_program program;
  t_service service;
};

BOOST_FIXTURE_TEST_SUITE(ServiceInternalObjectsGeneratorTests,
                         ServiceInternalObjectsGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(basic_service_includes_service_iface_in_header) {
  BOOST_CHECK_EQUAL(generateHeaderIncludes(),
                    "#include <iosfwd>\n"
                    "#include <cstdint>\n"
                    "\n"
                    "#include <boost/optional.hpp>\n"
                    "\n"
                    "#include \"ServiceNameIface.h\"\n"
                    "\n"
                    "namespace apache { namespace thrift { namespace protocol {\n"
                    "class TProtocol;\n"
                    "}}}\n");
}

BOOST_AUTO_TEST_CASE(basic_service_includes_all_needed_headers_in_source) {
  BOOST_CHECK_EQUAL(generateSourceIncludes(),
                    "#include <string>\n"
                    "#include <ostream>\n"
                    "\n"
                    "#include <thrift/protocol/TProtocol.h>\n"
                    "#include <thrift/TApplicationException.h>\n"
                    "#include <thrift/TToString.h>\n"
                    "#include <thrift/BoostOptionalUtils.h>\n");
}

BOOST_AUTO_TEST_SUITE_END()
