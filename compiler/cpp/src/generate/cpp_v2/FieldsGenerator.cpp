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

#include "FieldsGenerator.h"

#include <parse/t_field.h>
#include <parse/t_type.h>
#include <parse/t_base_type.h>

#include "IncludesGenerator.h"
#include "IncludesScanner.h"
#include "TypeGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

FieldsGenerator::FieldsGenerator(OutputGenerator& parent,
                                 const Fields& fields,
                                 const std::string& currentNamespace)
  : ContentsGenerator(parent),
    scanner_(new IncludesScanner()),
    typeGenerator_(new TypeGenerator(*this)),
    fields_(fields),
    requireOptional_(false) {
  typeGenerator_->setCurrentNamespace(currentNamespace);
}

FieldsGenerator::~FieldsGenerator() {
}

void FieldsGenerator::generateContents() {
  for (const auto& f : fields_)
    generateField(f);
}

void FieldsGenerator::generateHeader() {
  scanAllFieldsForHeaders();
  prepareHeader();
}

void FieldsGenerator::scanAllFieldsForHeaders() {
  for (const auto& f : fields_)
    scanForHeaders(f);
}

void FieldsGenerator::scanForHeaders(const t_field* field) {
  scanForOptional(field);
  scanTypeForHeaders(field->get_type());
}

void FieldsGenerator::scanForOptional(const t_field* field) {
  requireOptional_ |= (field->get_req() == t_field::T_OPTIONAL);
}

void FieldsGenerator::scanTypeForHeaders(const t_type* type) {
  if (type->is_void())
    throw std::string("INVALID TYPE - VOID type cannot be used for fields");
  scanner_->scan(type);
}

void FieldsGenerator::prepareHeader() {
  appendOptionalHeaderIfNeeded();
  appendScannedHeaders();
}

void FieldsGenerator::appendOptionalHeaderIfNeeded() {
  if (requireOptional_)
    indented() << systemInclude("boost/optional.hpp") << endl;
}

void FieldsGenerator::appendScannedHeaders() {
  IncludesGenerator gen(*this);
  gen.generate(*scanner_);
}

void FieldsGenerator::generateField(const t_field* field) {
  out() << indent();
  generateTypeForField(field);
  out() << " " << field->get_name() << "; // " << field->get_key() << endl;
}

void FieldsGenerator::generateTypeForField(const t_field* field) {
  if (field->get_req() == t_field::T_OPTIONAL)
    generateOptionalFieldType(field->get_type());
  else
    generateFieldType(field->get_type());
}

void FieldsGenerator::generateOptionalFieldType(const t_type* type) {
  out() << "boost::optional<";
  generateFieldType(type);
  out() << ">";
}

void FieldsGenerator::generateFieldType(const t_type* type) {
  typeGenerator_->generate(type);
}
