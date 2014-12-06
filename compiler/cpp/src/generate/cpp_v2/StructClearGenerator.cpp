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

#include "StructClearGenerator.h"

#include <ostream>
#include <algorithm>

#include <parse/t_struct.h>
#include <parse/t_field.h>

#include "DefaultConstructorGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

StructClearGenerator::StructClearGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : MethodGenerator(parent, tstruct->get_name()),
    struct_(tstruct),
    defaultConstructorGenerator_(new DefaultConstructorGenerator(parent, nullptr)) {
}

StructClearGenerator::~StructClearGenerator() {
}

void StructClearGenerator::generateReturnType() {
  out() << "void";
}

void StructClearGenerator::generateDeclarationWithoutReturn() {
  out() << "clear()";
}

void StructClearGenerator::generateDefinitionBody() {
  Fields optionalFields;
  Fields nonOptionalFields;

  splitFieldsByOptional(optionalFields, nonOptionalFields);

  processOptionalFields(optionalFields);
  processNonOptionalFields(nonOptionalFields);
}

bool StructClearGenerator::isOptional(const t_field* f) const {
  return defaultConstructorGenerator_->isOptional(f);
}

bool StructClearGenerator::hasDefaultValue(const t_field* f) const {
  return defaultConstructorGenerator_->hasDefaultValue(f);
}

void StructClearGenerator::generateClearForField(const t_field* f) {
  auto type = defaultConstructorGenerator_->getFieldType(f);
  if (defaultConstructorGenerator_->isSimpleType(type))
    return generateClearForSimpleField(f);
  generateClearForComplexType(f);
}

void StructClearGenerator::generateClearForComplexType(const t_field* f) {
  generateClearFieldByCallingClear(f);
  generateDefaultValueForComplexType(f);
}

void StructClearGenerator::generateClearForSimpleField(const t_field* f) {
  if (hasDefaultValue(f))
    return generateClearForSimpleFieldWithDefaultValue(f);
  generateClearForSimpleFieldWithoutDefaultValue(f);
}

void StructClearGenerator::generateClearForSimpleFieldWithoutDefaultValue(const t_field* f) {
  if (defaultConstructorGenerator_->getFieldType(f)->is_string())
    return generateClearFieldByCallingClear(f);
  generateClearFieldByAssigningEmpty(f);
}

void StructClearGenerator::generateClearFieldByAssigningEmpty(const t_field* f) {
  indented() << f->get_name() << " = {};" << endl;
}

void StructClearGenerator::generateClearFieldByCallingClear(const t_field* f) {
  indented() << f->get_name() << ".clear();" << endl;
}

void StructClearGenerator::generateClearForSimpleFieldWithDefaultValue(const t_field* f) {
  indented() << f->get_name() << " = ";
  defaultConstructorGenerator_->generateFieldValue(f);
  out() << ";" << endl;
}

void StructClearGenerator::generateOptionalReset(const t_field* f) {
  indented() << f->get_name() << " = boost::none;" << endl;
}

void StructClearGenerator::splitFieldsByOptional(Fields& optional, Fields& nonoptional) {
  auto fields = struct_->get_members();
  std::partition_copy(fields.begin(),
                      fields.end(),
                      std::back_inserter(optional),
                      std::back_inserter(nonoptional),
                      [this](const t_field* f) { return isOptional(f); });
}

void StructClearGenerator::processOptionalFields(const Fields& fields) {
  std::for_each(fields.begin(), fields.end(), [this](const t_field* f) {
    generateOptionalReset(f);
  });
  if (!fields.empty())
    out() << endl;
}

void StructClearGenerator::processNonOptionalFields(const Fields& fields) {
  std::for_each(fields.begin(), fields.end(), [this](const t_field* f) {
    generateClearForField(f);
  });
}

void StructClearGenerator::generateDefaultValueForComplexType(const t_field* f) {
  t_struct tmp(const_cast<t_program*>(struct_->get_program()));
  tmp.append(const_cast<t_field*>(f));
  DefaultConstructorGenerator gen(*this, &tmp);
  gen.generateDefinitionBody();
}
