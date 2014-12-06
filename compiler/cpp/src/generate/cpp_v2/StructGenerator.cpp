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

#include "StructGenerator.h"

#include <algorithm>

#include <parse/t_struct.h>

#include "CompleteStructMembersGenerator.h"
#include "FieldsGenerator.h"
#include "StructMembersGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

using MembersGeneratorFactory = StructMembersGeneratorFactoryTempl<CompleteStructMembersGenerator>;

std::string StructGenerator::getFileNameBase(const t_struct* tstruct) {
  return tstruct->get_name();
}

StructGenerator::HeaderGenerator::HeaderGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : StructDeclarationGenerator(parent,
                               StructMembersGeneratorFactoryPtr(new MembersGeneratorFactory),
                               tstruct) {
}

void StructGenerator::HeaderGenerator::generateHeader() {
  generateBasicHeaders();
  generateFieldsSpecificHeaders();
  generateThriftInternalHeaders();
  generateThriftForwardDecl();
}

void StructGenerator::HeaderGenerator::generateBasicHeaders() {
  indented() << systemInclude("iosfwd") << endl;
  indented() << systemInclude("cstdint") << endl;
}

void StructGenerator::HeaderGenerator::generateThriftInternalHeaders() {
  out() << endl;
  indented() << systemInclude("thrift/THashCombine.h") << endl;
  generateExceptionSpecificHeadersIfNeeded();
}

void StructGenerator::HeaderGenerator::generateExceptionSpecificHeadersIfNeeded() {
  if (getStruct()->is_xception())
    indented() << systemInclude("thrift/TException.h") << endl;
}

void StructGenerator::HeaderGenerator::generateFieldsSpecificHeaders() {
  FieldsGenerator gen(*this, getStruct()->get_members(), getCppNamespace(getStruct()));
  gen.generateHeader();
}

void StructGenerator::HeaderGenerator::generateThriftForwardDecl() {
  out() << endl;
  indented() << "namespace apache { namespace thrift { namespace protocol {" << endl;
  indented() << "class TProtocol;" << endl;
  indented() << "}}}" << endl;
}

StructGenerator::SourceGenerator::SourceGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : StructDefinitionGenerator(parent,
                              StructMembersGeneratorFactoryPtr(new MembersGeneratorFactory),
                              tstruct) {
}

void StructGenerator::SourceGenerator::generateHeader() {
  generateSwapAndMoveRequiredIncludes();
  generateReadWriteRequiredIncludes();
  generateOperatorOstreamRequiredIncludes();
}

void StructGenerator::SourceGenerator::generateReadWriteRequiredIncludes() {
  indented() << systemInclude("cstdint") << endl;
  indented() << systemInclude("string") << endl;
  indented() << systemInclude("thrift/protocol/TProtocol.h") << endl;
  indented() << systemInclude("thrift/TApplicationException.h") << endl;
  out() << endl;
}

void StructGenerator::SourceGenerator::generateSwapAndMoveRequiredIncludes() {
  indented() << systemInclude("utility") << endl;
  out() << endl;
}

void StructGenerator::SourceGenerator::generateOperatorOstreamRequiredIncludes() {
  indented() << systemInclude("ostream") << endl;
  indented() << systemInclude("thrift/TToString.h") << endl;
  if (isOptionalFieldUsed())
    indented() << systemInclude("thrift/BoostOptionalUtils.h") << endl;
}

bool StructGenerator::SourceGenerator::isOptionalFieldUsed() const {
  return std::any_of(getStruct()->get_members().begin(),
                     getStruct()->get_members().end(),
                     [](const t_field* f) { return f->get_req() == t_field::T_OPTIONAL; });
}
