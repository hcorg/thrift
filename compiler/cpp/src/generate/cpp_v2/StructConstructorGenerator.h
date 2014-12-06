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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCONSTRUCTORGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCONSTRUCTORGENERATOR_H_

#include "FunctionGenerator.h"

class t_struct;
class t_field;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class StructConstructorGenerator : public FunctionGenerator {
protected:
  StructConstructorGenerator(OutputGenerator& parent, const t_struct* tStruct);

public:
  void generateDefinition() override;
  void generateDeclaration() override;

protected:
  virtual void generateArguments() = 0;
  virtual void generateDefinitionBody() = 0;
  virtual void generateFieldValue(const t_field* f) = 0;

  const t_struct* getStruct() const { return tStruct_; }

private:
  void generateDefinitionSignature();
  void generateBody();
  void generateInitList();
  void generateFirstFieldInit();
  void generateTailFieldsInit();
  void generateFieldInit(const t_field* f);
  void generateInheritanceIfNeeded();
  void generateFirstFieldPrefix();

  const t_struct* tStruct_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCONSTRUCTORGENERATOR_H_
