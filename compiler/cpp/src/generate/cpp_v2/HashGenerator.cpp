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

#include "HashGenerator.h"

#include <parse/t_type.h>

#include <generate/ScopeGuard.h>

#include "NamespaceGuard.h"
#include "ClassScopeGuard.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

HashGenerator::HashGenerator(OutputGenerator& parent, const t_type* type)
  : OutputGenerator(parent), type_(type) {
}

void HashGenerator::generate() {
  NamespaceGuard ns(out(), "std");
  generateStruct();
}

void HashGenerator::generateStruct() {
  indented() << "template <>" << endl;
  indented() << "struct hash<" << getTypeNameWithNamespace() << ">";
  ClassScopeGuard g(*this);
  generateOperator();
}

void HashGenerator::generateOperator() {
  indented() << "std::size_t operator()(const " << getTypeNameWithNamespace() << "& obj) const";
  ScopeGuard g2(*this);
  generateOperatorBody();
}

std::string HashGenerator::getTypeNameWithNamespace() const {
  return namespacePrefix(type_) + type_->get_name();
}
