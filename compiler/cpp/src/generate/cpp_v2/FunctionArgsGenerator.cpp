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

#include "FunctionArgsGenerator.h"

#include <ostream>

#include <parse/t_field.h>
#include <parse/t_type.h>

#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

FunctionArgsGenerator::FunctionArgsGenerator(OutputGenerator& parent,
                                             const Fields& fields,
                                             const std::string& currentNamespace)
  : FieldsGenerator(parent, fields, currentNamespace), separatorNeeded_(false) {
}

void FunctionArgsGenerator::generateField(const t_field* field) {
  generateFieldSeparatorIfNeeded();
  generateTypeOrReference(field);
  out() << " " << field->get_name();
}

void FunctionArgsGenerator::generateTypeOrReference(const t_field* field) {
  if (isBasicType(field->get_type()))
    generateTypeForField(field);
  else
    generateTypeReference(field);
}

void FunctionArgsGenerator::generateTypeReference(const t_field* field) {
  out() << "const ";
  generateTypeForField(field);
  out() << "&";
}

void FunctionArgsGenerator::generateFieldSeparatorIfNeeded() {
  if (separatorNeeded_)
    out() << ", ";
  else
    separatorNeeded_ = true;
}
