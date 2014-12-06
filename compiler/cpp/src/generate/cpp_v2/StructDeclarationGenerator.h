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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTDECLARATIONGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTDECLARATIONGENERATOR_H_

#include <string>
#include <memory>

#include "ContentsGenerator.h"

class t_struct;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class StructMembersGeneratorFactory;
class StructMembersGenerator;

class StructDeclarationGenerator : public ContentsGenerator {
public:
  StructDeclarationGenerator(OutputGenerator& parent,
                             std::unique_ptr<StructMembersGeneratorFactory> membersGeneratorFactory,
                             const t_struct* tstruct);
  ~StructDeclarationGenerator();

  void generateHeader() override;
  void generateContents() override;
  void generateFooter() override;

protected:
  const t_struct* getStruct() const { return tStruct_; }

private:
  void generateStructLine();
  void generateStructBody();
  void generateStandaloneFunctions();
  void generateInheritanceIfNeeded();

  void generateFields();

  std::unique_ptr<StructMembersGenerator> buildMembersGenerator(const t_struct* tstruct);

  const t_struct* tStruct_;
  std::unique_ptr<StructMembersGeneratorFactory> membersGeneratorFactory_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTDECLARATIONGENERATOR_H_
