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

#include "CopyConstructorGenerator.h"

#include <ostream>

#include <parse/t_struct.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

CopyConstructorGenerator::CopyConstructorGenerator(OutputGenerator& parent, const t_struct* tStruct)
  : StructConstructorGenerator(parent, tStruct) {
}

void CopyConstructorGenerator::generateArguments() {
  out() << "const " << getStruct()->get_name() << "& other";
}

void CopyConstructorGenerator::generateDefinitionBody() {
  if (getStruct()->get_members().empty())
    indented() << "(void) other;" << endl;
}

void CopyConstructorGenerator::generateFieldValue(const t_field* f) {
  out() << "other." << f->get_name();
}
