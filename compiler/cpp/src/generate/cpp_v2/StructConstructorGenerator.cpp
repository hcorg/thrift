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

#include "StructConstructorGenerator.h"

#include <ostream>

#include <parse/t_struct.h>
#include <parse/t_field.h>

#include <generate/PrePostGuard.h>
#include <generate/IndentGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

StructConstructorGenerator::StructConstructorGenerator(OutputGenerator& parent,
                                                       const t_struct* tStruct)
  : FunctionGenerator(parent), tStruct_(tStruct) {
}

void StructConstructorGenerator::generateDefinition() {
  generateDefinitionSignature();
  generateInitList();
  generateBody();
}

void StructConstructorGenerator::generateDeclaration() {
  indented() << tStruct_->get_name() << "(";
  generateArguments();
  out() << ")";
}

void StructConstructorGenerator::generateDefinitionSignature() {
  indented() << tStruct_->get_name() << "::";
  generateDeclaration();
  out() << endl;
}

void StructConstructorGenerator::generateBody() {
  PrePostGuard g(*this, "{\n", "}\n");
  IndentGuard i(*this);
  generateDefinitionBody();
}

void StructConstructorGenerator::generateInitList() {
  IndentGuard g(*this);

  generateInheritanceIfNeeded();
  if (tStruct_->get_members().empty())
    return;

  generateFirstFieldInit();
  generateTailFieldsInit();
  out() << endl;
}

void StructConstructorGenerator::generateInheritanceIfNeeded() {
  if (tStruct_->is_xception()) {
    indented() << ": ::apache::thrift::TException{\"" << tStruct_->get_name()
               << " - Thrift Generated\"}";
    if (!tStruct_->get_members().empty())
      out() << ",";
    out() << endl;
  }
}

void StructConstructorGenerator::generateFirstFieldInit() {
  generateFirstFieldPrefix();
  generateFieldInit(tStruct_->get_members().front());
}

void StructConstructorGenerator::generateFirstFieldPrefix() {
  if (tStruct_->is_xception())
    out() << indent() << indent();
  else
    indented() << ": ";
}

void StructConstructorGenerator::generateTailFieldsInit() {
  IndentGuard g(*this);
  const auto& members = tStruct_->get_members();
  for (auto it = members.begin() + 1; it != members.end(); ++it) {
    out() << "," << endl;
    out() << indent();
    generateFieldInit(*it);
  }
}

void StructConstructorGenerator::generateFieldInit(const t_field* f) {
  out() << f->get_name() << "{";
  generateFieldValue(f);
  out() << "}";
}
