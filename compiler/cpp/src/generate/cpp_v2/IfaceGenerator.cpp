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

#include "IfaceGenerator.h"

#include <parse/t_service.h>

#include <generate/IndentGuard.h>

#include "IncludesGenerator.h"
#include "IncludesScanner.h"
#include "ServiceFunctionGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

IfaceGenerator::IfaceGenerator(OutputGenerator& parent, const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

IfaceGenerator::~IfaceGenerator() {
}

void IfaceGenerator::generateHeader() {
  scanIncludes();
  generateIncludes();
  generateExtendsInclude();
}

void IfaceGenerator::generateContents() {
  generateClassBegin();
  generateClassBody();
  generateClassEnd();
}

void IfaceGenerator::scanIncludes() {
  scanner_.reset(new IncludesScanner);
  scanFunctions();
}

void IfaceGenerator::scanFunctions() {
  for (auto f : service_->get_functions())
    scanFunction(f);
}

void IfaceGenerator::scanFunction(const t_function* f) {
  scanner_->scan(f->get_returntype());
  scanArguments(f->get_arglist());
  scanArguments(f->get_xceptions());
}

void IfaceGenerator::scanArguments(const t_struct* args) {
  for (auto m : args->get_members())
    scanner_->scan(m->get_type());
}

void IfaceGenerator::generateIncludes() {
  IncludesGenerator gen(*this);
  gen.generate(*scanner_);
}

void IfaceGenerator::generateExtendsInclude() {
  if (service_->get_extends())
    indented() << localInclude(getClassName(service_->get_extends()) + ".h") << endl;
}

void IfaceGenerator::generateClassBegin() {
  indented() << "class " << getClassName();
  appendInheritanceIfNeeded();
  out() << " {" << endl;
  indented() << "public:" << endl;
}

void IfaceGenerator::generateClassEnd() {
  indented() << "};";
}

void IfaceGenerator::generateClassBody() {
  IndentGuard g(*this);
  generateDestructor();
  generateFunctions();
}

void IfaceGenerator::generateDestructor() {
  indented() << "virtual ~" << getClassName() << "() {}" << endl;
  out() << endl;
}

std::string IfaceGenerator::getClassName(const t_service* service) {
  return service->get_name() + "Iface";
}

std::string IfaceGenerator::getClassName() const {
  return getFileNameBase(service_);
}

void IfaceGenerator::generateFunctions() {
  for (auto f : service_->get_functions())
    generateFunction(f);
}

void IfaceGenerator::generateFunction(const t_function* f) {
  ServiceFunctionGenerator gen(*this, getClassName(), f);
  gen.setCurrentNamespace(getCurrentNamespace());
  gen.setType(MethodGenerator::Type::PureVirtual);

  gen.generateDeclaration();

  out() << ";" << endl;
}

void IfaceGenerator::appendInheritanceIfNeeded() {
  if (!service_->get_extends())
    return;
  out() << " : public virtual " << namespacePrefixIfDifferent(service_, service_->get_extends())
        << getClassName(service_->get_extends());
}

std::string IfaceGenerator::getCurrentNamespace() const {
  return getCppNamespace(service_);
}
