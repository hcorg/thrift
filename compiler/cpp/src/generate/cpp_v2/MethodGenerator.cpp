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

#include "MethodGenerator.h"

#include <ostream>

#include <generate/ScopeGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

MethodGenerator::MethodGenerator(OutputGenerator& parent, const std::string& className)
  : FunctionGenerator(parent), className_(className), type_(Type::Standard) {
}

void MethodGenerator::generateDefinition() {
  throwIfGeneratingPureVirtualBody();
  appendIndent();
  appendReturnType();
  appendClassPrefix();
  generateDeclarationWithoutReturn();
  ScopeGuard g(*this);
  generateDefinitionBody();
}

void MethodGenerator::generateDeclaration() {
  appendIndent();
  appendVirtualIfNeeded();
  appendReturnType();
  generateDeclarationWithoutReturn();
  appendEndModifier();
}

void MethodGenerator::appendClassPrefix() {
  out() << className_ << "::";
}

void MethodGenerator::appendIndent() {
  out() << indent();
}

void MethodGenerator::appendReturnType() {
  generateReturnType();
  out() << " ";
}

void MethodGenerator::setType(Type type) {
  type_ = type;
}

void MethodGenerator::appendVirtualIfNeeded() {
  if (type_ == Type::Virtual || type_ == Type::PureVirtual)
    out() << "virtual ";
}

void MethodGenerator::appendEndModifier() {
  switch (type_) {
  case Type::Override:
    out() << " override";
    break;
  case Type::PureVirtual:
    out() << " = 0";
    break;
  default:
    break;
  }
}

void MethodGenerator::throwIfGeneratingPureVirtualBody() {
  if (type_ == Type::PureVirtual)
    throw std::string("MethodGenerator: Trying to generate body for pure virtual function");
  // C++ allows pure virtual functions to have bodies defined...
  // yet it is rarely used and leads to unreadable code
}
