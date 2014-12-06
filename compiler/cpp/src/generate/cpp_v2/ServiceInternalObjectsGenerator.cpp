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

#include "ServiceInternalObjectsGenerator.h"

#include <parse/t_service.h>

#include "ArgsRefsStructGenerator.h"
#include "ArgsStructGenerator.h"
#include "IfaceGenerator.h"
#include "ResultRefStructGenerator.h"
#include "ResultStructGenerator.h"
#include "ServiceInternalStructListGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

std::string ServiceInternalObjectsGenerator::getFileNameBase(const t_service* s) {
  return s->get_name() + "InternalObjects";
}

void ServiceInternalObjectsGenerator::fillContentsGenerator(ServiceInternalStructListGenerator& g) {
  g.appendGenerator<ArgsStructGenerator>();
  g.appendGenerator<ArgsRefsStructGenerator>();
  g.appendGenerator<ResultStructGenerator>();
  g.appendGenerator<ResultRefStructGenerator>();
}

ServiceInternalObjectsGenerator::HeaderGenerator::HeaderGenerator(OutputGenerator& parent,
                                                                  const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void ServiceInternalObjectsGenerator::HeaderGenerator::generateHeader() {
  generateSystemHeaders();
  out() << endl;
  generateServiceHeaders();
  out() << endl;
  generateTProtocolFwdDecl();
}

void ServiceInternalObjectsGenerator::HeaderGenerator::generateServiceHeaders() {
  indented() << localInclude(IfaceGenerator::getFileNameBase(service_) + ".h") << endl;
}

void ServiceInternalObjectsGenerator::HeaderGenerator::generateSystemHeaders() {
  indented() << systemInclude("iosfwd") << endl;
  indented() << systemInclude("cstdint") << endl;
  out() << endl;
  indented() << systemInclude("boost/optional.hpp") << endl;
}

void ServiceInternalObjectsGenerator::HeaderGenerator::generateTProtocolFwdDecl() {
  indented() << "namespace apache { namespace thrift { namespace protocol {" << endl;
  indented() << "class TProtocol;" << endl;
  indented() << "}}}" << endl;
}

void ServiceInternalObjectsGenerator::HeaderGenerator::generateContents() {
  ServiceInternalStructListGenerator gen(*this, service_);
  fillContentsGenerator(gen);
  gen.generateDeclarations();
}

ServiceInternalObjectsGenerator::SourceGenerator::SourceGenerator(OutputGenerator& parent,
                                                                  const t_service* service)
  : ContentsGenerator(parent), service_(service) {
}

void ServiceInternalObjectsGenerator::SourceGenerator::generateHeader() {
  generateSystemHeaders();
  generateThriftIncludes();
}

void ServiceInternalObjectsGenerator::SourceGenerator::generateContents() {
  ServiceInternalStructListGenerator gen(*this, service_);
  fillContentsGenerator(gen);
  gen.generateDefinitions();
}

void ServiceInternalObjectsGenerator::SourceGenerator::generateSystemHeaders() {
  indented() << systemInclude("string") << endl;
  indented() << systemInclude("ostream") << endl;
  out() << endl;
}

void ServiceInternalObjectsGenerator::SourceGenerator::generateThriftIncludes() {
  indented() << systemInclude("thrift/protocol/TProtocol.h") << endl;
  indented() << systemInclude("thrift/TApplicationException.h") << endl;
  indented() << systemInclude("thrift/TToString.h") << endl;
  indented() << systemInclude("thrift/BoostOptionalUtils.h") << endl;
}
