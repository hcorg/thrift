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

#include "ServiceClientGenerator.h"

#include <parse/t_service.h>

#include <generate/IndentGuard.h>

#include "ClientFunctionGenerator.h"
#include "IfaceGenerator.h"
#include "ServiceInternalObjectsGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

std::string ServiceClientGenerator::getFileNameBase(const t_service* service) {
  return service->get_name() + "Client";
}

ServiceClientGenerator::HeaderGenerator::HeaderGenerator(OutputGenerator& parent,
                                                         const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void ServiceClientGenerator::HeaderGenerator::generateHeader() {
  if (service_->get_extends())
    indented() << localInclude(getClassName(service_->get_extends()) + ".h") << endl;
  else
    indented() << systemInclude("thrift/client/ClientBase.h") << endl << endl;
  indented() << localInclude(IfaceGenerator::getClassName(service_) + ".h") << endl;
}

void ServiceClientGenerator::HeaderGenerator::generateContents() {
  generateClassDeclaration();
  generatePublicSection();
  generatePrivateSection();
  indented() << "};" << endl;
}

void ServiceClientGenerator::HeaderGenerator::generatePublicSection() {
  indented() << "public:" << endl;
  IndentGuard g(*this);
  generateConstructors();
  out() << endl;
  generatePublicDeclarations();
  out() << endl;
}

void ServiceClientGenerator::HeaderGenerator::generatePrivateSection() {
  indented() << "private:" << endl;
  IndentGuard g(*this);
  generatePrivateDeclarations();
}

void ServiceClientGenerator::HeaderGenerator::generatePrivateDeclarations() {
  for (auto f : service_->get_functions()) {
    ClientFunctionGenerator gen(*this, service_, f);
    gen.generatePrivateDeclarations();
    out() << endl;
  }
}

void ServiceClientGenerator::HeaderGenerator::generatePublicDeclarations() {
  for (auto f : service_->get_functions()) {
    ClientFunctionGenerator gen(*this, service_, f);
    gen.generatePublicDeclarations();
  }
}

void ServiceClientGenerator::HeaderGenerator::generateConstructors() {
  indented() << getClassName(service_) << "(TProtocolPtr prot)" << endl << indent() << "  : "
             << getParentClassName() << "(prot) {}" << endl;
  indented() << getClassName(service_) << "(TProtocolPtr iprot, TProtocolPtr oprot)" << endl
             << indent() << "  : " << getParentClassName() << "(iprot, oprot) {}" << endl;
}

void ServiceClientGenerator::HeaderGenerator::generateClassDeclaration() {
  indented() << "class " << getClassName(service_) << endl;
  IndentGuard g(*this);
  indented() << ": public " << getParentClassName() << ',' << endl;
  indented() << "  public virtual " << IfaceGenerator::getClassName(service_) << " {" << endl;
}

std::string ServiceClientGenerator::HeaderGenerator::getParentClassName() const {
  return service_->get_extends() ? getParentServiceName() : "::apache::thrift::client::ClientBase";
}

std::string ServiceClientGenerator::HeaderGenerator::getParentServiceName() const {
  return namespacePrefixIfDifferent(service_, service_->get_extends())
         + getClassName(service_->get_extends());
}

ServiceClientGenerator::SourceGenerator::SourceGenerator(OutputGenerator& parent,
                                                         const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void ServiceClientGenerator::SourceGenerator::generateHeader() {
  indented() << localInclude(ServiceInternalObjectsGenerator::getFileNameBase(service_) + ".h")
             << endl;
}

void ServiceClientGenerator::SourceGenerator::generateContents() {
  for (auto f : service_->get_functions()) {
    ClientFunctionGenerator gen(*this, service_, f);
    gen.generateDefinitions();
    out() << endl;
  }
}
