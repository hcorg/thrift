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

#include "ArgsStructMembersGenerator.h"

#include "DefaultConstructorGenerator.h"
#include "OperatorOstreamGenerator.h"
#include "StructClearGenerator.h"
#include "StructReadGenerator.h"
#include "StructWriteGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

ArgsStructMembersGenerator::ArgsStructMembersGenerator(OutputGenerator& parent,
                                                       const t_struct* tStruct)
  : StructMembersGenerator(parent, tStruct) {
}

void ArgsStructMembersGenerator::generateDefinitions() {
  doWith<DefaultConstructorGenerator>(appendDefinition);
  doWith<StructClearGenerator>(appendDefinition);
  doWith<StructReadGenerator>(appendDefinition);
  doWith<StructWriteGenerator>(appendDefinition);
  doWith<OperatorOstreamGenerator>(appendDefinition);
}

void ArgsStructMembersGenerator::generateBodyDeclarations() {
  doWith<DefaultConstructorGenerator>(appendDeclaration);
  out() << endl;
  doWith<StructClearGenerator>(appendDeclaration);
  out() << endl;
  doWith<StructReadGenerator>(appendDeclaration);
  doWith<StructWriteGenerator>(appendDeclaration);
}

void ArgsStructMembersGenerator::generateStandaloneDeclarations() {
  doWith<OperatorOstreamGenerator>(appendDeclaration);
}

void ArgsStructMembersGenerator::generateOtherNamespaceDeclarations() {
}
