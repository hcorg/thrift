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

#include "IncludesScanner.h"

#include <string>

#include <parse/t_type.h>
#include <parse/t_base_type.h>
#include <parse/t_list.h>
#include <parse/t_set.h>
#include <parse/t_map.h>

#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

namespace {

void throwOnInvalidType(const t_type* type) {
  throw std::string("INVALID TYPE in IncludesScanner: ") + type->get_name();
}
}

IncludesScanner::IncludesScanner()
  : cstdintRequired_(false),
    stringRequired_(false),
    vectorRequired_(false),
    setRequired_(false),
    mapRequired_(false) {
}

void IncludesScanner::scan(const t_type* type) {
  if (type->is_base_type())
    return scanBaseType(static_cast<const t_base_type*>(type));
  if (type->is_list())
    return scanListType(static_cast<const t_list*>(type));
  if (type->is_set())
    return scanSetType(static_cast<const t_set*>(type));
  if (type->is_map())
    return scanMapType(static_cast<const t_map*>(type));
  if (isUserDefinedType(type))
    return scanNamedType(type);
  throwOnInvalidType(type);
}

void IncludesScanner::scanBaseType(const t_base_type* type) {
  switch (type->get_base()) {
  case t_base_type::TYPE_STRING:
    stringRequired_ |= true;
    break;
  case t_base_type::TYPE_BYTE:
  case t_base_type::TYPE_I16:
  case t_base_type::TYPE_I32:
  case t_base_type::TYPE_I64:
    cstdintRequired_ |= true;
    break;
  case t_base_type::TYPE_DOUBLE:
  case t_base_type::TYPE_BOOL:
  case t_base_type::TYPE_VOID:
    // Nothing to do
    break;
  default:
    throwOnInvalidType(type);
  }
}

void IncludesScanner::scanNamedType(const t_type* type) {
  addUsedType(type->get_name());
}

void IncludesScanner::addUsedType(const std::string& name) {
  usedTypes_.insert(name);
}

void IncludesScanner::scanListType(const t_list* type) {
  vectorRequired_ = true;
  scan(type->get_elem_type());
}

void IncludesScanner::scanSetType(const t_set* type) {
  setRequired_ = true;
  scan(type->get_elem_type());
}

void IncludesScanner::scanMapType(const t_map* type) {
  mapRequired_ = true;
  scan(type->get_key_type());
  scan(type->get_val_type());
}
