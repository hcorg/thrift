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

#include "../FunctionGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

namespace {

class DummyGenerator : public FunctionGenerator {
public:
  DummyGenerator(OutputGenerator& parent) : FunctionGenerator(parent) {}

  void generateDeclaration() override { indented() << "void declaration()"; }

  void generateDefinitionBody() override { indented() << "body" << endl; }
};

std::string generate() {
  std::ostringstream stream;
  OutputGenerator parent(stream);
  DummyGenerator gen(parent);
  gen.generateDefinition();
  return stream.str();
}
} // namespace

BOOST_AUTO_TEST_CASE(function_generator_creates_indent_and_scope) {
  BOOST_CHECK_EQUAL(generate(),
                    "void declaration() {\n"
                    "  body\n"
                    "}\n");
}
