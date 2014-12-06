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

#include "HandlerFactoryGenerator.h"

#include <parse/t_service.h>

#include <generate/IndentGuard.h>

#include "IfaceGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

HandlerFactoryGenerator::HandlerFactoryGenerator(OutputGenerator& parent, const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void HandlerFactoryGenerator::generateHeader() {
  generateInclude();
  generateThriftFwdDecl();
}

void HandlerFactoryGenerator::generateContents() {
  indented() << "class " << getClassName();
  generateInheritance();
  out() << " {" << endl;
  indented() << "public:" << endl;
  generateClassContents();
  indented() << "};" << endl;
}

void HandlerFactoryGenerator::generateClassContents() {
  IndentGuard g(*this);
  generateUsing();
  generateDestructor();
  generateGetHandler();
  generateReleaseHandler();
}

void HandlerFactoryGenerator::generateGetHandler() {
  indented()
      << "virtual Handler* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;"
      << endl;
}

void HandlerFactoryGenerator::generateReleaseHandler() {
  indented() << "virtual void releaseHandler(Handler* handler) = 0;" << endl;
}

void HandlerFactoryGenerator::generateUsing() {
  indented() << "using Handler = " << IfaceGenerator::getClassName(service_) << ";" << endl;
  out() << endl;
}

void HandlerFactoryGenerator::generateDestructor() {
  indented() << "virtual ~" << getClassName() << "() {}" << endl;
  out() << endl;
}

void HandlerFactoryGenerator::generateIfaceInclude() {
  indented() << localInclude(IfaceGenerator::getClassName(service_) + ".h") << endl;
}

void HandlerFactoryGenerator::generateThriftFwdDecl() {
  indented() << "namespace apache { namespace thrift { struct TConnectionInfo; }}" << endl;
}

std::string HandlerFactoryGenerator::getClassName(const t_service* service) {
  return service->get_name() + "HandlerFactory";
}

std::string HandlerFactoryGenerator::getClassName() const {
  return getClassName(service_);
}

void HandlerFactoryGenerator::generateInclude() {
  if (isServiceExtended())
    generateParentInclude();
  generateIfaceInclude();
  out() << endl;
}

void HandlerFactoryGenerator::generateParentInclude() {
  indented() << localInclude(getClassName(service_->get_extends()) + ".h") << endl;
}

void HandlerFactoryGenerator::generateInheritance() {
  if (!isServiceExtended())
    return;
  out() << " : public virtual " << getParentClassName();
}

bool HandlerFactoryGenerator::isServiceExtended() const {
  return service_->get_extends() != nullptr;
}

std::string HandlerFactoryGenerator::getParentClassName() const {
  return namespacePrefixIfDifferent(service_, service_->get_extends())
         + getClassName(service_->get_extends());
}
