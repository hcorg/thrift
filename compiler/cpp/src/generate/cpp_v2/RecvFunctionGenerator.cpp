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

#include "RecvFunctionGenerator.h"

#include <parse/t_service.h>
#include <parse/t_function.h>

#include <generate/ScopeGuard.h>

#include "ResultRefStructBuilder.h"
#include "ServiceClientGenerator.h"
#include "TypeGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler::generate;

RecvFunctionGenerator::RecvFunctionGenerator(OutputGenerator& parent,
                                             const t_service* service,
                                             const t_function* function)
  : MethodGenerator(parent, ServiceClientGenerator::getClassName(service)),
    service_(service),
    function_(function) {
}

void RecvFunctionGenerator::generateDefinitionBody() {
  generateResultStruct();
  generateRecvMessageCall();
  generateResultProcessing();
}

void RecvFunctionGenerator::generateRecvMessageCall() {
  indented() << "recvMessage(\"" << function_->get_name() << "\", result);" << endl;
  out() << endl;
}

void RecvFunctionGenerator::generateResultStruct() {
  indented() << ResultRefStructBuilder(service_).buildName(function_) << " result;" << endl;
  generateResultAssignmentIfNeeded();
}

void RecvFunctionGenerator::generateReturnType() {
  if (isBasicReturnType())
    generateFunctionReturnType();
  else
    out() << "void";
}

std::string RecvFunctionGenerator::getGeneratedFunctionName() const {
  return "recv_" + function_->get_name();
}

void RecvFunctionGenerator::generateDeclarationWithoutReturn() {
  out() << getGeneratedFunctionName() << "(";
  generateArgumentIfNeeded();
  out() << ")";
}

void RecvFunctionGenerator::generateArgumentIfNeeded() {
  if (isBasicReturnType())
    return;
  generateFunctionReturnType();
  out() << "& result_";
}

void RecvFunctionGenerator::generateFunctionReturnType() {
  TypeGenerator typeGen(*this);
  typeGen.setCurrentNamespace(getCppNamespace(service_));

  typeGen.generate(function_->get_returntype());
}

bool RecvFunctionGenerator::isBasicReturnType() const {
  return isBasicType(function_->get_returntype());
}

void RecvFunctionGenerator::generateResultAssignmentIfNeeded() {
  if (isBasicReturnType())
    return;
  indented() << "result.success = result_;" << endl;
}

void RecvFunctionGenerator::generateResultProcessing() {
  generateSuccessProcessing();
  generateExceptionsProcessing();
  generateThrowOnUnknownResult();
}

void RecvFunctionGenerator::generateSuccessProcessing() {
  if (isVoidReturnType())
    return;
  indented() << "if (result.success)";
  ScopeGuard g(*this);
  generateSuccessReturn();
}

void RecvFunctionGenerator::generateSuccessReturn() {
  if (isBasicReturnType())
    indented() << "return *result.success;" << endl;
  else
    indented() << "return;" << endl;
}

void RecvFunctionGenerator::generateThrowOnUnknownResult() {
  if (isVoidReturnType() && function_->get_xceptions()->get_members().empty())
    return;
  indented() << "throw ::apache::thrift::TApplicationException("
             << "::apache::thrift::TApplicationException::MISSING_RESULT, " << '"'
             << service_->get_name() << "::" << function_->get_name()
             << " failed: unknown result\");" << endl;
}

bool RecvFunctionGenerator::isVoidReturnType() const {
  return function_->get_returntype()->is_void();
}

void RecvFunctionGenerator::generateExceptionsProcessing() {
  for (auto e : function_->get_xceptions()->get_members()) {
    auto name = "result." + e->get_name();
    indented() << "if (" << name << ")";
    ScopeGuard g(*this);
    indented() << "throw *" << name << ";" << endl;
  }
}
