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

#include "OperatorLtGenerator.h"

#include <parse/t_struct.h>

#include <generate/IndentGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

OperatorLtGenerator::OperatorLtGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : FunctionGenerator(parent), tStruct_(tstruct) {
}

void OperatorLtGenerator::generateDeclaration() {
  indented() << "bool operator<(const " << tStruct_->get_name() << "& a, const "
             << tStruct_->get_name() << "& b)";
}

void OperatorLtGenerator::generateDefinitionBody() {
  if (tStruct_->get_members().empty())
    return generateReturnFalse();
  generateCmpForMembers();
}

void OperatorLtGenerator::generateCmpForMembers() {
  generateCmpForAllButLastMember();
  generateCmpForLastMember();
}

void OperatorLtGenerator::generateCmpForAllButLastMember() {
  const auto& fields = tStruct_->get_members();
  for (auto it = fields.begin(); it != fields.end() - 1; ++it) {
    generateLtIf(*it);
    generateNeqIf(*it);
    out() << endl;
  }
}

void OperatorLtGenerator::generateCmpForLastMember() {
  indented() << "return ";
  generateCmp(tStruct_->get_members().back(), "<");
  out() << ";" << endl;
}

void OperatorLtGenerator::generateLtIf(const t_field* f) {
  generateIf(f, "<");
  generateIndentedReturnTrue();
}

void OperatorLtGenerator::generateNeqIf(const t_field* f) {
  generateIf(f, "!=");
  generateIndentedReturnFalse();
}

void OperatorLtGenerator::generateCmp(const t_field* f, const std::string& cmp) {
  out() << "a." << f->get_name() << " " << cmp << " b." << f->get_name();
}

void OperatorLtGenerator::generateIf(const t_field* f, const std::string& cmp) {
  indented() << "if (";
  generateCmp(f, cmp);
  out() << ")" << endl;
}

void OperatorLtGenerator::generateIndentedReturnTrue() {
  IndentGuard g(*this);
  indented() << "return true;" << endl;
}

void OperatorLtGenerator::generateIndentedReturnFalse() {
  IndentGuard g(*this);
  generateReturnFalse();
}

void OperatorLtGenerator::generateReturnFalse() {
  indented() << "return false;" << endl;
}
