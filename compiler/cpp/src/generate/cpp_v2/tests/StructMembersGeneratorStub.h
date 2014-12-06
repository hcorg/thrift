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

#ifndef _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_STRUCTMEMBERSGENERATORSTUB_H_
#define _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_STRUCTMEMBERSGENERATORSTUB_H_

#include <ostream>

#include <parse/t_struct.h>

#include "../StructMembersGenerator.h"

class StructMembersGeneratorStub
    : public apache::thrift::compiler::generate::cpp_v2::StructMembersGenerator {
public:
  StructMembersGeneratorStub(OutputGenerator& parent, const t_struct* tStruct)
    : StructMembersGenerator(parent, tStruct) {}

  void generateDefinitions() override { indented() << getStructName() << ": definitions" << endl; }

  void generateBodyDeclarations() override {
    indented() << getStructName() << ": body declarations" << endl;
  }

  void generateStandaloneDeclarations() override {
    indented() << getStructName() << ": standalone declarations" << endl;
  }

  void generateOtherNamespaceDeclarations() override {
    indented() << getStructName() << ": other namespace declarations" << endl;
  }

  void generateFields() override { indented() << getStructName() << ": fields" << endl; }

private:
  std::string getStructName() { return getStruct()->get_name(); }
};

using apache::thrift::compiler::generate::cpp_v2::StructMembersGeneratorFactoryTempl;
using StructMembersGeneratorStubFactory
    = StructMembersGeneratorFactoryTempl<StructMembersGeneratorStub>;

#endif // _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_STRUCTMEMBERSGENERATORSTUB_H_
