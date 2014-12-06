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

#include "EnumGenerator.h"

#include <ostream>

#include <parse/t_enum.h>

#include "ClassScopeGuard.h"
#include "EnumHashGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

EnumGenerator::EnumGenerator(OutputGenerator& parent, const t_enum* tenum)
  : ContentsGenerator(parent), tEnum_(tenum) {
}

void EnumGenerator::generateHeader() {
  indented() << systemInclude("cstdint") << endl;
  indented() << systemInclude("functional") << endl;
}

void EnumGenerator::generateContents() {
  out() << "enum class " << tEnum_->get_name() << " : std::int32_t";
  ClassScopeGuard g(*this);
  generateConstants();
}

void EnumGenerator::generateConstants() {
  for (const auto& constant : tEnum_->get_constants())
    generateConstant(constant);
}

void EnumGenerator::generateConstant(const t_enum_value* constant) {
  indented() << constant->get_name() << " = " << constant->get_value() << "," << endl;
}

void EnumGenerator::generateFooter() {
  EnumHashGenerator gen(*this, tEnum_);
  gen.generate();
}

std::string EnumGenerator::getFileNameBase(const t_enum* tenum) {
  return tenum->get_name();
}
