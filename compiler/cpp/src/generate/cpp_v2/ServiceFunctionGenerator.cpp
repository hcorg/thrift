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

#include "ServiceFunctionGenerator.h"

#include <parse/t_function.h>

#include "TypeGenerator.h"
#include "FunctionArgsGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

ServiceFunctionGenerator::ServiceFunctionGenerator(OutputGenerator& parent,
                                                   const std::string& className,
                                                   const t_function* function)
  : MethodGenerator(parent, className), function_(function) {
}

void ServiceFunctionGenerator::generateReturnType() {
  TypeGenerator gen(*this);
  gen.setCurrentNamespace(currentNamespace_);
  gen.generate(function_->get_returntype());
}

void ServiceFunctionGenerator::generateDeclarationWithoutReturn() {
  out() << getGeneratedFunctionName() << "(";
  generateArguments();
  out() << ")";
}

void ServiceFunctionGenerator::generateDefinitionBody() {
}

std::string ServiceFunctionGenerator::getGeneratedFunctionName() const {
  return getFunction()->get_name();
}

void ServiceFunctionGenerator::generateArguments() {
  FunctionArgsGenerator gen(*this, function_->get_arglist()->get_members(), currentNamespace_);
  gen.generateContents();
}

void ServiceFunctionGenerator::setCurrentNamespace(const std::string& currentNamespace) {
  currentNamespace_ = currentNamespace;
}
