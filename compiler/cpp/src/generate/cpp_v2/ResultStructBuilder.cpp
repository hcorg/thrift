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

#include "ResultStructBuilder.h"

#include <parse/t_field.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

ResultStructBuilder::ResultStructBuilder(const t_service* service)
  : ServiceInternalStructBuilder(service) {
}

ResultStructBuilder::~ResultStructBuilder() {
  if (struct_)
    for (auto m : struct_->get_members())
      delete m;
}

void ResultStructBuilder::appendField(t_field* f) {
  f->set_req(t_field::T_OPTIONAL);
  struct_->append(f);
}

const t_struct* ResultStructBuilder::build(const t_function* f) {
  struct_.reset(new t_struct(const_cast<t_program*>(getService()->get_program()), buildName(f)));
  appendSuccessIfNotNull(f);
  appendAllExceptions(f);
  return struct_.get();
}

void ResultStructBuilder::appendAllExceptions(const t_function* f) {
  for (auto m : f->get_xceptions()->get_members())
    appendField(new t_field{*m});
}

void ResultStructBuilder::appendSuccessIfNotNull(const t_function* f) {
  if (f->get_returntype()->is_void())
    return;
  appendField(new t_field(f->get_returntype(), "success", 0));
}

std::string ResultStructBuilder::buildName(const t_function* f) {
  return getService()->get_name() + "_" + f->get_name() + "_result";
}
