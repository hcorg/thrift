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

#include "TypeGenerator.h"

#include <ostream>
#include <string>

#include <parse/t_base_type.h>
#include <parse/t_type.h>
#include <parse/t_set.h>
#include <parse/t_map.h>
#include <parse/t_list.h>

#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

TypeGenerator::TypeGenerator(OutputGenerator& parent) : OutputGenerator(parent) {
}

void TypeGenerator::generate(const t_type* type) {
  throwIfNull(type);
  if (type->is_base_type())
    return generateBaseType(static_cast<const t_base_type*>(type));
  if (type->is_list())
    return generateListType(static_cast<const t_list*>(type));
  if (type->is_set())
    return generateSetType(static_cast<const t_set*>(type));
  if (type->is_map())
    return generateMapType(static_cast<const t_map*>(type));
  if (isUserDefinedType(type))
    return generateNamedType(type);
  throwOnUnknownType(type);
}

void TypeGenerator::throwIfNull(const t_type* type) {
  if (type == nullptr)
    throw std::string("UNKNOWN TYPE in TypeGenerator");
}

void TypeGenerator::generateBaseType(const t_base_type* type) {
  switch (type->get_base()) {
  case t_base_type::TYPE_I8:
    out() << "std::int8_t";
    return;
  case t_base_type::TYPE_I16:
    out() << "std::int16_t";
    return;
  case t_base_type::TYPE_I32:
    out() << "std::int32_t";
    return;
  case t_base_type::TYPE_I64:
    out() << "std::int64_t";
    return;
  case t_base_type::TYPE_STRING:
    out() << "std::string";
    return;
  case t_base_type::TYPE_DOUBLE:
    out() << "double";
    return;
  case t_base_type::TYPE_BOOL:
    out() << "bool";
    return;
  case t_base_type::TYPE_VOID:
    out() << "void";
    return;
  }
  throwOnUnknownType(type);
}

void TypeGenerator::throwOnUnknownType(const t_type* type) {
  throw std::string("UNKNOWN TYPE in TypeGenerator: ") + type->get_name();
}

void TypeGenerator::generateNamedType(const t_type* type) {
  out() << getOptionalNamespacePrefix(type) << type->get_name();
}

void TypeGenerator::setCurrentNamespace(const std::string& ns) {
  currentNamespace_ = ns;
}

std::string TypeGenerator::getOptionalNamespacePrefix(const t_type* type) {
  return namespacePrefixIfDifferent(currentNamespace_, getCppNamespace(type));
}

void TypeGenerator::generateListType(const t_list* type) {
  if (type->has_cpp_name())
    return generateSubsitutedType(type);

  out() << "std::vector<";
  generate(type->get_elem_type());
  out() << ">";
}

void TypeGenerator::generateSetType(const t_set* type) {
  if (type->has_cpp_name())
    return generateSubsitutedType(type);

  out() << "std::unordered_set<";
  generate(type->get_elem_type());
  out() << ">";
}

void TypeGenerator::generateMapType(const t_map* type) {
  if (type->has_cpp_name())
    return generateSubsitutedType(type);

  out() << "std::unordered_map<";
  generate(type->get_key_type());
  out() << ", ";
  generate(type->get_val_type());
  out() << ">";
}

void TypeGenerator::generateSubsitutedType(const t_container* cont) {
  out() << cont->get_cpp_name();
}
