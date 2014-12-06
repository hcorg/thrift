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

#include "DefaultConstructorGenerator.h"

#include <parse/t_const_value.h>
#include <parse/t_list.h>
#include <parse/t_map.h>
#include <parse/t_set.h>
#include <parse/t_struct.h>

#include "Utils.h"
#include "TypeGenerator.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

DefaultConstructorGenerator::DefaultConstructorGenerator(OutputGenerator& parent,
                                                         const t_struct* tStruct)
  : StructConstructorGenerator(parent, tStruct) {
}

void DefaultConstructorGenerator::generateArguments() {
}

void DefaultConstructorGenerator::generateDefinitionBody() {
  for (auto f : getStruct()->get_members())
    if (hasDefaultValue(f))
      generateBodyForMember(f);
}

void DefaultConstructorGenerator::generateFieldValue(const t_field* f) {
  if (!hasDefaultValue(f))
    return;

  const auto type = getFieldType(f);

  if (isSimpleType(type))
    return generateValueForSimpleType(type, f->get_value());
}

void DefaultConstructorGenerator::generateValueForSimpleType(const t_type* type,
                                                             const t_const_value* v) {
  if (type->is_base_type())
    return generateValueForBaseType(v);
  if (type->is_enum())
    return generateValueForEnumType(type, v);
}

void DefaultConstructorGenerator::generateBodyForMember(const t_field* f) {
  if (isSimpleType(getFieldType(f)))
    return;
  generateBodyForField(std::string(), f, f->get_value());
}

void DefaultConstructorGenerator::generateBodyForField(const std::string& parentPrefix,
                                                       const t_field* f,
                                                       const t_const_value* val) {
  initializeOptionalComplexField(f, parentPrefix);
  generateBodyForType(getFieldPrefix(f, parentPrefix), getFieldType(f), val);
}

void DefaultConstructorGenerator::generateBodyForType(const std::string& fieldPrefix,
                                                      const t_type* type,
                                                      const t_const_value* val) {
  if (type->is_list())
    return generateListInit(fieldPrefix, static_cast<const t_list*>(type), val);
  if (type->is_set())
    return generateSetInit(fieldPrefix, static_cast<const t_set*>(type), val);
  if (type->is_map())
    return generateMapInit(fieldPrefix, static_cast<const t_map*>(type), val);
  if (type->is_struct() || type->is_xception())
    return generateStructInit(fieldPrefix, static_cast<const t_struct*>(type), val);
}

void DefaultConstructorGenerator::generateListInit(const std::string& parentPrefix,
                                                   const t_list* l,
                                                   const t_const_value* v) {
  if (v->get_type() != t_const_value::CV_LIST)
    throw "INVALID LIST INIT in DefaultConstructorGenerator: " + std::to_string(v->get_type());

  const auto elemType = l->get_elem_type()->get_true_type();

  for (auto e : v->get_list()) {
    generateListElement(parentPrefix, elemType, e);
  }
}

void DefaultConstructorGenerator::generateListElement(const std::string& listPrefix,
                                                      const t_type* elemType,
                                                      const t_const_value* v) {
  if (isSimpleType(elemType)) {
    indented() << listPrefix << "push_back(";
    generateValueForSimpleType(elemType, v);
    out() << ");" << endl;
  } else {
    indented() << listPrefix << "emplace_back();" << endl;
    generateBodyForType(listPrefix + "back().", elemType, v);
  }
}

void DefaultConstructorGenerator::generateSetInit(const std::string& parentPrefix,
                                                  const t_set* s,
                                                  const t_const_value* v) {
  if (v->get_type() != t_const_value::CV_LIST)
    throw "INVALID SET INIT in DefaultConstructorGenerator: " + std::to_string(v->get_type());

  const auto elemType = s->get_elem_type()->get_true_type();

  for (auto e : v->get_list()) {
    generateSetElement(parentPrefix, elemType, e);
  }
}

void DefaultConstructorGenerator::generateSetElement(const std::string& setPrefix,
                                                     const t_type* elemType,
                                                     const t_const_value* v) {
  if (isSimpleType(elemType)) {
    indented() << setPrefix << "insert(";
    generateValueForSimpleType(elemType, v);
    out() << ");" << endl;
  } else {
    const auto tmpVar = makeUniqueName("tmp");
    generateTempItemInitialization(tmpVar, elemType, v);
    indented() << setPrefix << "insert(std::move(" << tmpVar << "));" << endl;
  }
}

void DefaultConstructorGenerator::generateTempItemInitialization(const std::string& name,
                                                                 const t_type* type,
                                                                 const t_const_value* value) {
  out() << indent();
  generateTypeName(type);
  out() << " " << name << ";" << endl;
  generateBodyForType(name + ".", type, value);
}

void DefaultConstructorGenerator::generateMapInit(const std::string& parentPrefix,
                                                  const t_map* m,
                                                  const t_const_value* val) {
  if (val->get_type() != t_const_value::CV_MAP)
    throw "INVALID MAP INIT in DefaultConstructorGenerator: " + std::to_string(val->get_type());

  const auto keyType = m->get_key_type()->get_true_type();
  const auto valType = m->get_val_type()->get_true_type();

  for (const auto& v : val->get_map())
    generateMapElement(parentPrefix, keyType, valType, v.first, v.second);
}

void DefaultConstructorGenerator::generateTempItemInitIfNeeded(const std::string& name,
                                                               const t_type* type,
                                                               const t_const_value* value) {
  if (!isSimpleType(type))
    generateTempItemInitialization(name, type, value);
}

void DefaultConstructorGenerator::generateSimpleValueOrMove(const std::string& name,
                                                            const t_type* type,
                                                            const t_const_value* value) {
  if (isSimpleType(type))
    generateValueForSimpleType(type, value);
  else
    out() << "std::move(" << name << ")";
}

void DefaultConstructorGenerator::generateMapElement(const std::string& mapPrefix,
                                                     const t_type* keyType,
                                                     const t_type* valueType,
                                                     const t_const_value* key,
                                                     const t_const_value* value) {
  const auto keyVarName = makeUniqueName("tmpKey");
  const auto valVarName = makeUniqueName("tmpVal");

  generateTempItemInitIfNeeded(keyVarName, keyType, key);
  generateTempItemInitIfNeeded(valVarName, valueType, value);

  indented() << mapPrefix << "emplace(";

  generateSimpleValueOrMove(keyVarName, keyType, key);
  out() << ", ";
  generateSimpleValueOrMove(valVarName, valueType, value);

  out() << ");" << endl;
}

void DefaultConstructorGenerator::generateStructInit(const std::string& fieldPrefix,
                                                     const t_struct* s,
                                                     const t_const_value* val) {
  if (val->get_type() != t_const_value::CV_MAP)
    throw "INVALID STRUCT INIT in DefaultConstructorGenerator: " + std::to_string(val->get_type());

  for (const auto& v : val->get_map())
    generateStructFieldInit(fieldPrefix, getStructField(s, v.first), v.second);
}

void DefaultConstructorGenerator::generateTypeName(const t_type* type) {
  TypeGenerator gen(*this);
  gen.setCurrentNamespace(getCppNamespace(getStruct()));
  gen.generate(type);
}

void DefaultConstructorGenerator::initializeOptionalComplexField(const t_field* f,
                                                                 const std::string& parentPrefix) {
  if (!isOptional(f))
    return;

  indented() << parentPrefix << f->get_name() << " = ";
  generateTypeName(f->get_type());
  out() << "{};" << endl;
}

bool DefaultConstructorGenerator::isOptional(const t_field* f) const {
  return f->get_req() == t_field::T_OPTIONAL;
}

std::string DefaultConstructorGenerator::getFieldPrefix(const t_field* f,
                                                        const std::string& parent) const {
  return parent + f->get_name() + (isOptional(f) ? "->" : ".");
}

void DefaultConstructorGenerator::generateStructFieldInit(const std::string& fieldPrefix,
                                                          const t_field* field,
                                                          const t_const_value* value) {
  const auto type = getFieldType(field);
  if (isSimpleType(type)) {
    indented() << fieldPrefix << field->get_name() << " = ";
    generateValueForSimpleType(type, value);
    out() << ";" << endl;
  } else {
    return generateBodyForField(fieldPrefix, field, value);
  }
}

bool DefaultConstructorGenerator::hasDefaultValue(const t_field* f) const {
  return f->get_value() != nullptr;
}

void DefaultConstructorGenerator::generateValueForEnumType(const t_type* e,
                                                           const t_const_value* v) {
  const auto enumName = namespacePrefix(e) + e->get_name();

  switch (v->get_type()) {
  case t_const_value::CV_IDENTIFIER:
    out() << enumName << "::" << v->get_identifier_name();
    break;
  case t_const_value::CV_INTEGER:
    out() << "static_cast<" << enumName << ">(" << v->get_integer() << ")";
    break;
  default:
    throw "INVALID ENUM TYPE in DefaultConstructorGenerator: " + std::to_string(v->get_type());
  }
}

void DefaultConstructorGenerator::generateValueForBaseType(const t_const_value* v) {
  switch (v->get_type()) {
  case t_const_value::CV_INTEGER:
    out() << v->get_integer();
    break;
  case t_const_value::CV_DOUBLE:
    out() << v->get_double();
    break;
  case t_const_value::CV_STRING:
    out() << '"' << v->get_string() << '"';
    break;
  default:
    throw "INVALID VALUE TYPE in DefaultConstructorGenerator: " + std::to_string(v->get_type());
  }
}

std::string DefaultConstructorGenerator::getStructFieldName(const t_const_value* v) const {
  if (v->get_type() != t_const_value::CV_STRING)
    throw "INVALID FIELD TYPE in DefaultConstructorGenerator: " + std::to_string(v->get_type());
  return v->get_string();
}

const t_field* DefaultConstructorGenerator::getStructField(const t_struct* s,
                                                           const t_const_value* v) const {
  const auto name = getStructFieldName(v);

  const auto f = s->get_field_by_name(name);
  if (!f)
    throw "INVALID FIELD NAME in DefaultConstructorGenerator: " + name;
  return f;
}

bool DefaultConstructorGenerator::isSimpleType(const t_type* type) const {
  return type->is_base_type() || type->is_enum();
}

const t_type* DefaultConstructorGenerator::getFieldType(const t_field* f) const {
  return f->get_type()->get_true_type();
}
