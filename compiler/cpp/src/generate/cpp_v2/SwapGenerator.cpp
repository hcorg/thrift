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

#include "SwapGenerator.h"

#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

SwapGenerator::SwapGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : FunctionGenerator(parent), tStruct_(tstruct) {
}

void SwapGenerator::generateDeclaration() {
  indented() << "void swap(" << tStruct_->get_name() << "& a, " << tStruct_->get_name() << "& b)";
}

void SwapGenerator::generateDefinitionBody() {
  indented() << "using ::std::swap;" << endl;
  generateParameterSuppressIfEmpty();
  generateSwapForFields();
}

void SwapGenerator::generateParameterSuppressIfEmpty() {
  if (tStruct_->get_members().empty()) {
    indented() << "(void) a;" << endl;
    indented() << "(void) b;" << endl;
  }
}

void SwapGenerator::generateSwapForFields() {
  for (auto f : tStruct_->get_members())
    generateSwapForField(f);
}

void SwapGenerator::generateSwapForField(const t_field* f) {
  indented() << "swap(a." << f->get_name() << ", b." << f->get_name() << ");" << endl;
}
