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

#include "../StructGenerator.h"

#include <parse/t_struct.h>
#include <parse/t_base_type.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct SourceGeneratorTestsFixture {

  SourceGeneratorTestsFixture() : tstruct(nullptr, "StructName") {}

  std::string generateSourceIncludes() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructGenerator::SourceGenerator gen(parent, &tstruct);
    gen.generateHeader();
    return stream.str();
  }

  std::string generateHeaderIncludes() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    StructGenerator::HeaderGenerator gen(parent, &tstruct);
    gen.generateHeader();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(SourceGeneratorTests, SourceGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(empty_struct_add_basic_includes_to_source) {
  BOOST_CHECK_EQUAL(generateSourceIncludes(),
                    "#include <utility>\n"
                    "\n"
                    "#include <cstdint>\n"
                    "#include <string>\n"
                    "#include <thrift/protocol/TProtocol.h>\n"
                    "#include <thrift/TApplicationException.h>\n"
                    "\n"
                    "#include <ostream>\n"
                    "#include <thrift/TToString.h>\n");
}

BOOST_AUTO_TEST_CASE(struct_with_optional_field_add_more_includes_to_source) {
  t_field f(t_base_type::type_i32, "f", 1);
  f.set_req(t_field::T_OPTIONAL);
  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateSourceIncludes(),
                    "#include <utility>\n"
                    "\n"
                    "#include <cstdint>\n"
                    "#include <string>\n"
                    "#include <thrift/protocol/TProtocol.h>\n"
                    "#include <thrift/TApplicationException.h>\n"
                    "\n"
                    "#include <ostream>\n"
                    "#include <thrift/TToString.h>\n"
                    "#include <thrift/BoostOptionalUtils.h>\n");
}

BOOST_AUTO_TEST_CASE(empty_struct_add_basic_includes_to_header) {
  BOOST_CHECK_EQUAL(generateHeaderIncludes(),
                    "#include <iosfwd>\n"
                    "#include <cstdint>\n"
                    "\n"
                    "#include <thrift/THashCombine.h>\n"
                    "\n"
                    "namespace apache { namespace thrift { namespace protocol {\n"
                    "class TProtocol;\n"
                    "}}}\n");
}

BOOST_AUTO_TEST_CASE(exception_adds_exception_includes_to_header) {
  tstruct.set_xception(true);
  BOOST_CHECK_EQUAL(generateHeaderIncludes(),
                    "#include <iosfwd>\n"
                    "#include <cstdint>\n"
                    "\n"
                    "#include <thrift/THashCombine.h>\n"
                    "#include <thrift/TException.h>\n"
                    "\n"
                    "namespace apache { namespace thrift { namespace protocol {\n"
                    "class TProtocol;\n"
                    "}}}\n");
}

BOOST_AUTO_TEST_SUITE_END()
