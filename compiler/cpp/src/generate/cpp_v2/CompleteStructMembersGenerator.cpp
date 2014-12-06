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

#include "CompleteStructMembersGenerator.h"

#include <ostream>

#include <parse/t_struct.h>

#include "CopyConstructorGenerator.h"
#include "DefaultConstructorGenerator.h"
#include "MoveConstructorGenerator.h"

#include "AssignmentOperatorGenerator.h"
#include "MoveAssignmentOperatorGenerator.h"

#include "OperatorEqGenerator.h"
#include "OperatorLtGenerator.h"
#include "OperatorOstreamGenerator.h"
#include "SwapGenerator.h"

#include "StructClearGenerator.h"

#include "StructReadGenerator.h"
#include "StructWriteGenerator.h"

#include "StructHashGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

CompleteStructMembersGenerator::CompleteStructMembersGenerator(OutputGenerator& parent,
                                                               const t_struct* tStruct)
  : StructMembersGenerator(parent, tStruct) {
}

void CompleteStructMembersGenerator::generateDefinitions() {
  doWithAllConstructors(appendDefinition);
  doWithAllAssignmentOperators(appendDefinition);
  doWith<StructClearGenerator>(appendDefinition);
  doWithAllProtocolMethods(appendDefinition);
  doWithAllStandaloneFunctions(appendDefinition);
}

void CompleteStructMembersGenerator::generateBodyDeclarations() {
  doWithAllConstructors(appendDeclaration);
  out() << endl;
  doWithAllAssignmentOperators(appendDeclaration);
  out() << endl;
  doWith<StructClearGenerator>(appendDeclaration);
  out() << endl;
  doWithAllProtocolMethods(appendDeclaration);
}

void CompleteStructMembersGenerator::generateStandaloneDeclarations() {
  doWithAllStandaloneFunctions(appendDeclaration);
  generateOperatorNeq();
}

template <typename Fun>
void CompleteStructMembersGenerator::doWithAllStandaloneFunctions(Fun fun) {
  doWith<OperatorEqGenerator>(fun);
  // TODO: enable when std::map/set used  doWith<OperatorLtGenerator>(fun);
  doWith<SwapGenerator>(fun);
  doWith<OperatorOstreamGenerator>(fun);
}

template <typename Fun>
void CompleteStructMembersGenerator::doWithAllAssignmentOperators(Fun fun) {
  doWith<AssignmentOperatorGenerator>(fun);
  doWith<MoveAssignmentOperatorGenerator>(fun);
}

template <typename Fun>
void CompleteStructMembersGenerator::doWithAllConstructors(Fun fun) {
  doWith<DefaultConstructorGenerator>(fun);
  doWith<CopyConstructorGenerator>(fun);
  doWith<MoveConstructorGenerator>(fun);
}

void CompleteStructMembersGenerator::generateOtherNamespaceDeclarations() {
  StructHashGenerator gen(*this, getStruct());
  gen.generate();
}

void CompleteStructMembersGenerator::generateOperatorNeq() {
  indented() << "inline bool operator!=(const " << getStruct()->get_name() << "& a, const "
             << getStruct()->get_name() << "& b) { return !(a == b); }" << endl;
}

template <typename Fun>
void CompleteStructMembersGenerator::doWithAllProtocolMethods(Fun fun) {
  doWith<StructReadGenerator>(fun);
  doWith<StructWriteGenerator>(fun);
}
