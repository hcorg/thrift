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

#include "OperatorEqGenerator.h"

#include <parse/t_struct.h>

#include <generate/IndentGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

OperatorEqGenerator::OperatorEqGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : FunctionGenerator(parent), tStruct_(tstruct) {
}

void OperatorEqGenerator::generateDeclaration() {
  indented() << "bool operator==(const " << tStruct_->get_name() << "& a, const "
             << tStruct_->get_name() << "& b)";
}

void OperatorEqGenerator::generateDefinitionBody() {
  if (tStruct_->get_members().empty())
    return generateBodyForEmptyStruct();
  generateBodyForMembers();
}

void OperatorEqGenerator::generateBodyForMembers() {
  indented() << "return ";
  generateBodyForStructWithMembers();
  out() << ";" << endl;
}

void OperatorEqGenerator::generateBodyForEmptyStruct() {
  indented() << "(void) a;" << endl;
  indented() << "(void) b;" << endl;
  indented() << "return true;" << endl;
}

void OperatorEqGenerator::generateBodyForStructWithMembers() {
  const auto& fields = tStruct_->get_members();
  generateSingleFieldCmp(fields.front());
  IndentGuard g(*this);
  for (auto it = fields.begin() + 1; it != fields.end(); ++it) {
    out() << endl << indent() << "&& ";
    generateSingleFieldCmp(*it);
  }
}

void OperatorEqGenerator::generateSingleFieldCmp(const t_field* f) {
  out() << "a." << f->get_name() << " == b." << f->get_name();
}
