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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCLEARGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCLEARGENERATOR_H_

#include <string>
#include <vector>
#include <memory>

#include "MethodGenerator.h"

class t_struct;
class t_field;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class DefaultConstructorGenerator;

class StructClearGenerator : public MethodGenerator {
public:
  StructClearGenerator(OutputGenerator& parent, const t_struct* tstruct);
  ~StructClearGenerator();

  void generateReturnType() override;
  void generateDeclarationWithoutReturn() override;
  void generateDefinitionBody() override;

private:
  bool isOptional(const t_field* f) const;
  bool hasDefaultValue(const t_field* f) const;

  void generateOptionalReset(const t_field* f);
  void generateClearForField(const t_field* f);

  void generateClearForSimpleField(const t_field* f);
  void generateClearForSimpleFieldWithDefaultValue(const t_field* f);
  void generateClearForSimpleFieldWithoutDefaultValue(const t_field* f);

  void generateClearFieldByCallingClear(const t_field* f);
  void generateClearFieldByAssigningEmpty(const t_field* f);

  void generateClearForComplexType(const t_field* f);

  void generateDefaultValueForComplexType(const t_field* f);

  using Fields = std::vector<t_field*>;

  void splitFieldsByOptional(Fields& optional, Fields& nonoptional);

  void processOptionalFields(const Fields& f);
  void processNonOptionalFields(const Fields& f);

  const t_struct* struct_;
  std::unique_ptr<DefaultConstructorGenerator> defaultConstructorGenerator_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTCLEARGENERATOR_H_
