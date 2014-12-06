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

#include "ArgsStructBuilder.h"

#include <string>

#include <parse/t_function.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

ArgsStructBuilder::ArgsStructBuilder(const t_service* service)
  : ServiceInternalStructBuilder(service) {
}

ArgsStructBuilder::~ArgsStructBuilder() {
}

const t_struct* ArgsStructBuilder::build(const t_function* f) {
  struct_.reset(new t_struct{*f->get_arglist()});
  struct_->set_name(buildName(f));
  return struct_.get();
}

std::string ArgsStructBuilder::buildName(const t_function* f) {
  return getService()->get_name() + '_' + f->get_name() + "_args";
}
