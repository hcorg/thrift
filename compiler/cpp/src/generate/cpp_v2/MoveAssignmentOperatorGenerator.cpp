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

#include "MoveAssignmentOperatorGenerator.h"

#include <ostream>

#include <parse/t_struct.h>
#include <parse/t_field.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

MoveAssignmentOperatorGenerator::MoveAssignmentOperatorGenerator(OutputGenerator& parent,
                                                                 const t_struct* tstruct)
  : AssignmentOperatorGenerator(parent, tstruct) {
}

std::string MoveAssignmentOperatorGenerator::getArgumentType(const std::string& className) {
  return className + "&&";
}

void MoveAssignmentOperatorGenerator::generateFieldAssignment(const t_field* f) {
  if (f->get_req() == t_field::T_OPTIONAL)
    generateOptionalFieldAssignment(f);
  else
    generateRequiredFieldAssignment(f);
}

void MoveAssignmentOperatorGenerator::generateOptionalFieldAssignment(const t_field* f) {
  indented() << f->get_name() << ".swap(other." << f->get_name() << ");" << endl;
}

void MoveAssignmentOperatorGenerator::generateRequiredFieldAssignment(const t_field* f) {
  indented() << f->get_name() << " = std::move(other." << f->get_name() << ");" << endl;
}
