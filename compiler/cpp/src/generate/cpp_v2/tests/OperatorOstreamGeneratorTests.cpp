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

#include "../OperatorOstreamGenerator.h"

#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

struct OperatorOstreamGeneratorTestsFixture {

  OperatorOstreamGeneratorTestsFixture() : tstruct(nullptr) { tstruct.set_name("StructName"); }

  std::string generateBody() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    OperatorOstreamGenerator gen(parent, &tstruct);
    gen.generateDefinitionBody();
    return stream.str();
  }

  std::string generateDeclaration() {
    std::ostringstream stream;
    OutputGenerator parent(stream);
    OperatorOstreamGenerator gen(parent, &tstruct);
    gen.generateDeclaration();
    return stream.str();
  }

  t_struct tstruct;
};

BOOST_FIXTURE_TEST_SUITE(OperatorOstreamGeneratorTests, OperatorOstreamGeneratorTestsFixture)

BOOST_AUTO_TEST_CASE(struct_name_is_used_in_declaration) {
  BOOST_CHECK_EQUAL(generateDeclaration(),
                    "std::ostream& operator<<(std::ostream& out, const StructName& obj)");
}

BOOST_AUTO_TEST_CASE(empty_struct_prints_its_name) {
  BOOST_CHECK_EQUAL(generateBody(),
                    "(void) obj;\n"
                    "using apache::thrift::to_string;\n"
                    "out << \"StructName(\";\n"
                    "out << \")\";\n"
                    "return out;\n");
}

BOOST_AUTO_TEST_CASE(single_field_struct_prints_that_field) {
  t_field f(nullptr, "f", 1);
  tstruct.append(&f);

  BOOST_CHECK_EQUAL(generateBody(),
                    "using apache::thrift::to_string;\n"
                    "out << \"StructName(\";\n"
                    "out << \"f=\" << to_string(obj.f);\n"
                    "out << \")\";\n"
                    "return out;\n");
}

BOOST_AUTO_TEST_CASE(multiple_field_struct_prints_all_fields) {
  t_field f1(nullptr, "f1", 1);
  tstruct.append(&f1);
  t_field f2(nullptr, "f2", 2);
  tstruct.append(&f2);
  t_field f3(nullptr, "f3", 3);
  tstruct.append(&f3);

  BOOST_CHECK_EQUAL(generateBody(),
                    "using apache::thrift::to_string;\n"
                    "out << \"StructName(\";\n"
                    "out << \"f1=\" << to_string(obj.f1);\n"
                    "out << \", f2=\" << to_string(obj.f2);\n"
                    "out << \", f3=\" << to_string(obj.f3);\n"
                    "out << \")\";\n"
                    "return out;\n");
}

BOOST_AUTO_TEST_SUITE_END()
