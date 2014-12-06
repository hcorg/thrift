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

#include "AssignmentOperatorGenerator.h"

#include <ostream>
#include <string>

#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

AssignmentOperatorGenerator::AssignmentOperatorGenerator(OutputGenerator& parent,
                                                         const t_struct* tstruct)
  : MethodGenerator(parent, tstruct->get_name()), tStruct_(tstruct) {
}

void AssignmentOperatorGenerator::generateReturnType() {
  out() << tStruct_->get_name() << "&";
}

void AssignmentOperatorGenerator::generateDeclarationWithoutReturn() {
  out() << "operator=(" << getArgumentType(tStruct_->get_name()) << " other)";
}

std::string AssignmentOperatorGenerator::getArgumentType(const std::string& className) {
  return "const " + className + "&";
}

void AssignmentOperatorGenerator::generateDefinitionBody() {
  generateParameterSuppressIfEmpty();
  generateMembersAssignment();
  generateReturnStatement();
}

void AssignmentOperatorGenerator::generateMembersAssignment() {
  for (auto f : tStruct_->get_members())
    generateFieldAssignment(f);
}

void AssignmentOperatorGenerator::generateReturnStatement() {
  indented() << "return *this;" << endl;
}

void AssignmentOperatorGenerator::generateFieldAssignment(const t_field* f) {
  indented() << f->get_name() << " = other." << f->get_name() << ";" << endl;
}

void AssignmentOperatorGenerator::generateParameterSuppressIfEmpty() {
  if (tStruct_->get_members().empty())
    indented() << "(void) other;" << endl;
}
