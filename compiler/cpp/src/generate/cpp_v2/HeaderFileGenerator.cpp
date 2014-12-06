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

#include "HeaderFileGenerator.h"

#include <algorithm>
#include <cctype>

#include <generate/PrePostGuard.h>

#include "NamespaceGuard.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

HeaderFileGenerator::HeaderFileGenerator(std::ostream& out,
                                         const std::string& moduleName,
                                         const std::string& thriftNamespaceName)
  : OutputGenerator(out), moduleName_(moduleName), thriftNamespaceName_(thriftNamespaceName) {
}

void HeaderFileGenerator::generate(ContentsGenerator& innerGenerator) {
  out() << autogenComment() << endl;
  PrePostGuard ifdefGuard(*this, getIfdefOpen(), getIfdefClose());
  generateHeader(innerGenerator);
  generateContents(innerGenerator);
  generateFooter(innerGenerator);
}

void HeaderFileGenerator::generateHeader(ContentsGenerator& innerGenerator) {
  PrePostGuard endlGuard(*this, endl, endl);
  innerGenerator.generateHeader();
}

void HeaderFileGenerator::generateFooter(ContentsGenerator& innerGenerator) {
  out() << endl;
  innerGenerator.generateFooter();
}

void HeaderFileGenerator::generateContents(ContentsGenerator& innerGenerator) {
  if (thriftNamespaceName_.empty())
    return innerGenerator.generateContents();
  NamespaceGuard namespaceGuard(out(), thriftNamespaceName_);
  PrePostGuard endlGuard(*this, endl, endl);
  innerGenerator.generateContents();
}

std::string HeaderFileGenerator::getIfdefOpen() const {
  const auto def = getDefineName();
  const auto ret = "#ifndef " + def + endl;
  return ret + "#define " + def + endl;
}

std::string HeaderFileGenerator::getIfdefClose() const {
  return endl + "#endif // " + getDefineName() + endl;
}

namespace {
std::string toUpper(std::string in) {
  std::transform(in.begin(), in.end(), in.begin(), [](char c) { return std::toupper(c); });
  return in;
}
} // namespace

std::string HeaderFileGenerator::getDefineName() const {
  return "THRIFT_GENERATED_CPP_V2_" + toUpper(moduleName_) + "_H";
}
