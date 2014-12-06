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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_IFACEGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_IFACEGENERATOR_H_

#include <memory>

#include "ContentsGenerator.h"

class t_service;
class t_function;
class t_struct;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class IncludesScanner;

class IfaceGenerator : public ContentsGenerator {
public:
  IfaceGenerator(OutputGenerator& parent, const t_service* service);
  ~IfaceGenerator();

  void generateHeader() override;
  void generateContents() override;

  static std::string getFileNameBase(const t_service* service) { return getClassName(service); }
  static std::string getClassName(const t_service* service);

private:
  void scanIncludes();
  void scanFunctions();
  void scanFunction(const t_function* f);
  void scanArguments(const t_struct* args);

  void generateIncludes();
  void generateExtendsInclude();

  void generateClassBegin();
  void generateClassEnd();
  void generateClassBody();
  void generateDestructor();

  void generateFunctions();
  void generateFunction(const t_function* f);

  void appendInheritanceIfNeeded();

  std::string getClassName() const;
  std::string getCurrentNamespace() const;

  const t_service* service_;
  std::unique_ptr<IncludesScanner> scanner_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_IFACEGENERATOR_H_
