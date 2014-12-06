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

#include "SingletonHandlerFactoryGenerator.h"

#include <parse/t_service.h>

#include <generate/IndentGuard.h>

#include "HandlerFactoryGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

SingletonHandlerFactoryGenerator::SingletonHandlerFactoryGenerator(OutputGenerator& parent,
                                                                   const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void SingletonHandlerFactoryGenerator::generateHeader() {
  indented() << systemInclude("memory") << endl;
  out() << endl;
  indented() << localInclude(getParentClassName() + ".h") << endl;
}

void SingletonHandlerFactoryGenerator::generateContents() {
  generateClassBegin();
  generatePublicSection();
  generatePrivateSection();
  generateClassEnd();
}

std::string SingletonHandlerFactoryGenerator::getClassName(const t_service* service) {
  return service->get_name() + "SingletonHandlerFactory";
}

std::string SingletonHandlerFactoryGenerator::getClassName() const {
  return getClassName(service_);
}

std::string SingletonHandlerFactoryGenerator::getParentClassName() const {
  return HandlerFactoryGenerator::getClassName(service_);
}

void SingletonHandlerFactoryGenerator::generateClassBegin() {
  indented() << "class " << getClassName() << " : public virtual " << getParentClassName() << " {"
             << endl;
}

void SingletonHandlerFactoryGenerator::generateClassEnd() {
  indented() << "};" << endl;
}

void SingletonHandlerFactoryGenerator::generatePublicSection() {
  indented() << "public:" << endl;
  IndentGuard g(*this);
  generateConstructor();
  generateGetHandler();
  generateReleaseHandler();
}

void SingletonHandlerFactoryGenerator::generateReleaseHandler() {
  indented() << "void releaseHandler(Handler*) override {}" << endl;
  out() << endl;
}

void SingletonHandlerFactoryGenerator::generateGetHandler() {
  indented() << "Handler* getHandler(const ::apache::thrift::TConnectionInfo&) override {" << endl;
  indented() << indent() << "return handler_.get();" << endl;
  indented() << "}" << endl;
  out() << endl;
}

void SingletonHandlerFactoryGenerator::generateConstructor() {
  indented() << getClassName() << "(const std::shared_ptr<Handler>& handler)" << endl;
  indented() << indent() << ": handler_(handler) {}" << endl;
  out() << endl;
}

void SingletonHandlerFactoryGenerator::generatePrivateSection() {
  indented() << "private:" << endl;
  IndentGuard g(*this);
  indented() << "std::shared_ptr<Handler> handler_;" << endl;
}
