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

#include "t_base_type.h"

namespace {

t_base_type type_void_("void", t_base_type::TYPE_VOID);
t_base_type type_string_("string", t_base_type::TYPE_STRING);
t_base_type type_binary_("string", t_base_type::TYPE_STRING);
t_base_type type_slist_("string", t_base_type::TYPE_STRING);
t_base_type type_bool_("bool", t_base_type::TYPE_BOOL);
t_base_type type_byte_("byte", t_base_type::TYPE_BYTE);
t_base_type type_i16_("i16", t_base_type::TYPE_I16);
t_base_type type_i32_("i32", t_base_type::TYPE_I32);
t_base_type type_i64_("i64", t_base_type::TYPE_I64);
t_base_type type_double_("double", t_base_type::TYPE_DOUBLE);

t_base_type* build_binary() {
  type_binary_.set_binary(true);
  return &type_binary_;
}

t_base_type* build_slist() {
  type_slist_.set_string_list(true);
  return &type_slist_;
}

} // namespace

t_base_type* t_base_type::type_void = &type_void_;
t_base_type* t_base_type::type_string = &type_string_;
t_base_type* t_base_type::type_binary = build_binary();
t_base_type* t_base_type::type_slist = build_slist();
t_base_type* t_base_type::type_bool = &type_bool_;
t_base_type* t_base_type::type_byte = &type_byte_;
t_base_type* t_base_type::type_i16 = &type_i16_;
t_base_type* t_base_type::type_i32 = &type_i32_;
t_base_type* t_base_type::type_i64 = &type_i64_;
t_base_type* t_base_type::type_double = &type_double_;
