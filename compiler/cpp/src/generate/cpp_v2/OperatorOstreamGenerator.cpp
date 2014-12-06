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

#include "OperatorOstreamGenerator.h"

#include <algorithm>

#include <parse/t_struct.h>
#include <parse/t_field.h>

#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

OperatorOstreamGenerator::OperatorOstreamGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : FunctionGenerator(parent), tStruct_(tstruct) {
}

void OperatorOstreamGenerator::generateDeclaration() {
  indented() << "std::ostream& operator<<(std::ostream& out, const " << tStruct_->get_name()
             << "& obj)";
}

void OperatorOstreamGenerator::generateDefinitionBody() {
  generateParameterSuppressIfEmpty();
  generateHeader();
  generateOutputForAllItems();
  generateFooter();
}

void OperatorOstreamGenerator::generateHeader() {
  indented() << "using apache::thrift::to_string;" << endl;
  indented() << "out << \"" << tStruct_->get_name() << "(\";" << endl;
}

void OperatorOstreamGenerator::generateFooter() {
  indented() << "out << \")\";" << endl;
  indented() << "return out;" << endl;
}

void OperatorOstreamGenerator::generateParameterSuppressIfEmpty() {
  if (tStruct_->get_members().empty())
    indented() << "(void) obj;" << endl;
}

void OperatorOstreamGenerator::generateOutputForAllItems() {
  const auto beg = tStruct_->get_members().begin();
  const auto end = tStruct_->get_members().end();

  for (auto it = beg; it != end; ++it) {
    indented() << "out << \"";

    if (it != beg) {
      out() << ", ";
    }

    generateField(*it);
  }
}

void OperatorOstreamGenerator::generateField(const t_field* f) {
  out() << f->get_name() << "=\""
        << " << to_string(obj." << f->get_name() << ");" << endl;
}
