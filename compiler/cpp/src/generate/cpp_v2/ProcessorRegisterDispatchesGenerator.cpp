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

#include "ProcessorRegisterDispatchesGenerator.h"

#include <ostream>

#include <generate/ScopeGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

ProcessorRegisterDispatchesGenerator::ProcessorRegisterDispatchesGenerator(OutputGenerator& parent,
                                                                           const t_service* service)
  : OutputGenerator(parent), service_(service) {
}

void ProcessorRegisterDispatchesGenerator::generate() {
  generateDeclaration();
  ScopeGuard g(*this);
  generateBody();
}

void ProcessorRegisterDispatchesGenerator::generateDeclaration() {
  indented() << "template <typename DispatcherType>" << endl;
  indented() << "void registerDispatches(DispatcherType& dispatcher)";
}

void ProcessorRegisterDispatchesGenerator::generateBody() {
  if (isServiceEmpty())
    return generateBodyForEmptyService();
}

void ProcessorRegisterDispatchesGenerator::generateBodyForEmptyService() {
  indented() << "(void) dispatcher;" << endl;
}

bool ProcessorRegisterDispatchesGenerator::isServiceEmpty() const {
  return true;
}
