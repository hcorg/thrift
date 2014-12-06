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

#include "StructDeclarationGenerator.h"

#include <parse/t_struct.h>

#include "ClassScopeGuard.h"
#include "StructMembersGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

StructDeclarationGenerator::StructDeclarationGenerator(
    OutputGenerator& parent,
    std::unique_ptr<StructMembersGeneratorFactory> factory,
    const t_struct* tstruct)
  : ContentsGenerator(parent), tStruct_(tstruct), membersGeneratorFactory_(std::move(factory)) {
}

StructDeclarationGenerator::~StructDeclarationGenerator() {
}

void StructDeclarationGenerator::generateHeader() {
}

void StructDeclarationGenerator::generateContents() {
  generateStructLine();
  generateStructBody();
  out() << endl;
  generateStandaloneFunctions();
}

void StructDeclarationGenerator::generateStructLine() {
  indented() << "struct " << tStruct_->get_name();
  generateInheritanceIfNeeded();
}

void StructDeclarationGenerator::generateInheritanceIfNeeded() {
  if (tStruct_->is_xception())
    out() << " : public ::apache::thrift::TException";
}

void StructDeclarationGenerator::generateStructBody() {
  ClassScopeGuard guard(*this);
  auto gen = buildMembersGenerator(tStruct_);
  gen->generateBodyDeclarations();
  out() << endl;
  generateFields();
}

void StructDeclarationGenerator::generateFields() {
  auto gen = buildMembersGenerator(tStruct_);
  gen->generateFields();
}

void StructDeclarationGenerator::generateStandaloneFunctions() {
  auto gen = buildMembersGenerator(tStruct_);
  gen->generateStandaloneDeclarations();
}

void StructDeclarationGenerator::generateFooter() {
  auto gen = buildMembersGenerator(tStruct_);
  gen->generateOtherNamespaceDeclarations();
}

std::unique_ptr<StructMembersGenerator> StructDeclarationGenerator::buildMembersGenerator(
    const t_struct* tstruct) {
  return membersGeneratorFactory_->createGenerator(*this, tstruct);
}
