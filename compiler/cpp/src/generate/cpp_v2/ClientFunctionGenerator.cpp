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

#include "ClientFunctionGenerator.h"

#include <algorithm>

#include <parse/t_function.h>
#include <parse/t_service.h>

#include <generate/IndentGuard.h>

#include "RecvFunctionGenerator.h"
#include "SendFunctionGenerator.h"
#include "ServiceClientGenerator.h"
#include "ServiceFunctionGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

class ClientFunctionGenerator::MainClientFunctionGenerator : public ServiceFunctionGenerator {
public:
  MainClientFunctionGenerator(ClientFunctionGenerator& parent)
    : ServiceFunctionGenerator(parent,
                               ServiceClientGenerator::getClassName(parent.service_),
                               parent.function_),
      parent_(parent) {
    setCurrentNamespace(getCppNamespace(parent.service_));
    setType(MethodGenerator::Type::Override);
  }

private:
  void generateDefinitionBody() override {
    IndentGuard g(parent_);
    parent_.setIndent(getIndent());
    parent_.generateMainFunctionBody();
  }

  ClientFunctionGenerator& parent_;
};

class ClientFunctionGenerator::HelperClientFunctionGenerator : public ServiceFunctionGenerator {
public:
  HelperClientFunctionGenerator(ClientFunctionGenerator& parent)
    : ServiceFunctionGenerator(parent,
                               ServiceClientGenerator::getClassName(parent.service_),
                               parent.function_),
      parent_(parent) {
    setCurrentNamespace(getCppNamespace(parent.service_));
  }

  void generateComplexReturnType() { ServiceFunctionGenerator::generateReturnType(); }

private:
  void generateDefinitionBody() override {
    IndentGuard g(parent_);
    parent_.setIndent(getIndent());
    parent_.generateMainBodyForSimpleType();
  }

  void generateReturnType() override { out() << "void"; }

  void generateArguments() override {
    generateComplexReturnType();
    out() << "& result_";
    generateNormalArguments();
  }

  void generateNormalArguments() {
    parent_.generateCommaWhenFunctionHasArguments();
    ServiceFunctionGenerator::generateArguments();
  }

  ClientFunctionGenerator& parent_;
};

ClientFunctionGenerator::ClientFunctionGenerator(OutputGenerator& parent,
                                                 const t_service* service,
                                                 const t_function* function)
  : OutputGenerator(parent),
    service_(service),
    function_(function),
    mainGenerator(new MainClientFunctionGenerator(*this)),
    helperGenerator(new HelperClientFunctionGenerator(*this)),
    recvGenerator(new RecvFunctionGenerator(*this, service_, function_)),
    sendGenerator(new SendFunctionGenerator(*this, service_, function_)) {
}

ClientFunctionGenerator::~ClientFunctionGenerator() {
}

void ClientFunctionGenerator::generatePublicDeclarations() {
  generateMainDeclaration();
  generateHelperDeclaration();
}

void ClientFunctionGenerator::generateMainDeclaration() {
  mainGenerator->generateDeclaration();
  generateDeclSeparator();
}

void ClientFunctionGenerator::generateHelperDeclaration() {
  if (isFunctionReturningBasicType())
    return;
  helperGenerator->generateDeclaration();
  generateDeclSeparator();
}

void ClientFunctionGenerator::generatePrivateDeclarations() {
  generateSendDeclaration();
  generateRecvDeclaration();
}

void ClientFunctionGenerator::generateSendDeclaration() {
  sendGenerator->generateDeclaration();
  generateDeclSeparator();
}

void ClientFunctionGenerator::generateRecvDeclaration() {
  if (isFunctionOneway())
    return;
  recvGenerator->generateDeclaration();
  generateDeclSeparator();
}

void ClientFunctionGenerator::generateDefinitions() {
  generatePublicDefinitions();
  out() << endl;
  generatePrivateDefinitions();
}

void ClientFunctionGenerator::generateDeclSeparator() {
  out() << ";" << endl;
}

void ClientFunctionGenerator::generatePublicDefinitions() {
  generateMainDefinition();
  generateHelperDefinition();
}

void ClientFunctionGenerator::generateMainDefinition() {
  mainGenerator->generateDefinition();
}

void ClientFunctionGenerator::generateHelperDefinition() {
  if (isFunctionReturningBasicType())
    return;
  out() << endl;
  helperGenerator->generateDefinition();
}

void ClientFunctionGenerator::generatePrivateDefinitions() {
  generateSendDefinition();
  generateRecvDefinition();
}

void ClientFunctionGenerator::generateSendDefinition() {
  sendGenerator->generateDefinition();
}

void ClientFunctionGenerator::generateRecvDefinition() {
  if (isFunctionOneway())
    return;
  out() << endl;
  recvGenerator->generateDefinition();
}

bool ClientFunctionGenerator::isFunctionOneway() const {
  return function_->is_oneway();
}

bool ClientFunctionGenerator::isFunctionReturningBasicType() const {
  return isBasicType(function_->get_returntype());
}

void ClientFunctionGenerator::generateSendArgs() {
  auto args = function_->get_arglist()->get_members();
  if (args.empty())
    return;

  out() << args.front()->get_name();
  std::for_each(args.begin() + 1, args.end(), [this](const t_field* f) {
    out() << ", " << f->get_name();
  });
}

void ClientFunctionGenerator::generateSendCall() {
  indented() << sendGenerator->getGeneratedFunctionName() << "(";
  generateSendArgs();
  out() << ");" << endl;
}

std::string ClientFunctionGenerator::getReturnIfNeeded() const {
  const bool notNullReturn = !function_->get_returntype()->is_void();
  return (notNullReturn && isFunctionReturningBasicType()) ? "return " : "";
}

void ClientFunctionGenerator::generateRecvCall() {
  if (isFunctionOneway())
    return;
  indented() << getReturnIfNeeded() << recvGenerator->getGeneratedFunctionName() << "("
             << getRecvArg() << ");" << endl;
}

std::string ClientFunctionGenerator::getRecvArg() const {
  return isFunctionReturningBasicType() ? "" : "result_";
}

void ClientFunctionGenerator::generateMainFunctionBody() {
  if (isFunctionReturningBasicType())
    return generateMainBodyForSimpleType();
  generateMainBodyForComplexType();
}

void ClientFunctionGenerator::generateMainBodyForSimpleType() {
  generateSendCall();
  generateRecvCall();
}

void ClientFunctionGenerator::generateMainBodyForComplexType() {
  generateHelperResult();
  generateHelperCall();
  indented() << "return result;" << endl;
}

void ClientFunctionGenerator::generateHelperResult() {
  out() << indent();
  helperGenerator->generateComplexReturnType();
  out() << " result;" << endl;
}

void ClientFunctionGenerator::generateHelperCall() {
  indented() << function_->get_name() << "(result";
  generateCommaWhenFunctionHasArguments();
  generateSendArgs();
  out() << ");" << endl;
}

void ClientFunctionGenerator::generateCommaWhenFunctionHasArguments() {
  if (function_->get_arglist()->get_members().empty())
    return;
  out() << ", ";
}
