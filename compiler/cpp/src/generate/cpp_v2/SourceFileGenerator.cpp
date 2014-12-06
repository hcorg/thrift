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

#include "SourceFileGenerator.h"

#include <ostream>

#include <generate/PrePostGuard.h>

#include "NamespaceGuard.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

SourceFileGenerator::SourceFileGenerator(std::ostream& out,
                                         const std::string& moduleName,
                                         const std::string& thriftNamespaceName)
  : OutputGenerator(out), moduleName_(moduleName), thriftNamespaceName_(thriftNamespaceName) {
}

void SourceFileGenerator::generate(ContentsGenerator& innerGenerator) {
  out() << autogenComment() << endl;
  generateModuleInclude();
  generateHeader(innerGenerator);
  generateContents(innerGenerator);
}

void SourceFileGenerator::generateHeader(ContentsGenerator& innerGenerator) {
  innerGenerator.generateHeader();
  out() << endl;
}

void SourceFileGenerator::generateContents(ContentsGenerator& innerGenerator) {
  if (thriftNamespaceName_.empty())
    return innerGenerator.generateContents();
  NamespaceGuard namespaceGuard(out(), thriftNamespaceName_);
  PrePostGuard endlGuard(*this, endl, endl);
  innerGenerator.generateContents();
}

void SourceFileGenerator::generateModuleInclude() {
  out() << localInclude(moduleName_ + ".h") << endl << endl;
}
