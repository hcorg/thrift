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

#include "StructHashGenerator.h"

#include <parse/t_struct.h>
#include <parse/t_field.h>

#include <generate/IndentGuard.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

StructHashGenerator::StructHashGenerator(OutputGenerator& parent, const t_struct* type)
  : HashGenerator(parent, type), tStruct_(type) {
}

void StructHashGenerator::generateOperatorBody() {
  if (tStruct_->get_members().empty())
    return generateForEmptyStruct();
  generateForStructWithFields();
}

void StructHashGenerator::generateHashForMembers() {
  for (auto f : tStruct_->get_members())
    generateHashCombine(f);
}

void StructHashGenerator::generateHashCombine(const t_field* f) {
  if (f->get_req() == t_field::T_OPTIONAL)
    return generateHashCombineOptional(f);
  generateHashCombineRequired(f);
}

void StructHashGenerator::generateForEmptyStruct() {
  indented() << "(void) obj;" << endl;
  indented() << "return 0;" << endl;
}

void StructHashGenerator::generateForStructWithFields() {
  indented() << "using apache::thrift::hashCombine;" << endl;
  indented() << "std::size_t h = 0;" << endl;
  generateHashForMembers();
  indented() << "return h;" << endl;
}

void StructHashGenerator::generateHashCombineRequired(const t_field* f) {
  indented() << "hashCombine(h, obj." << f->get_name() << ");" << endl;
}

void StructHashGenerator::generateHashCombineOptional(const t_field* f) {
  indented() << "if (obj." << f->get_name() << ")" << endl;
  IndentGuard g(*this);
  indented() << "hashCombine(h, *obj." << f->get_name() << ");" << endl;
}
