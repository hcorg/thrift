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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTREADGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTREADGENERATOR_H_

#include <string>

#include "MethodGenerator.h"

class t_struct;
class t_field;
class t_type;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class StructReadGenerator : public MethodGenerator {
public:
  StructReadGenerator(OutputGenerator& parent, const t_struct* tstruct);

  void generateReturnType() override;
  void generateDeclarationWithoutReturn() override;
  void generateDefinitionBody() override;

public: // for tests
  void generateFieldRead(const t_field* f);

private:
  void generateVariables();
  void generateStructClear();
  void generateStructBegin();
  void generateStructEnd();
  void generateWhileLoop();
  void generateReturnStatement();

  void generateWhileLoopBody();
  void generateReadFieldBegin();
  void generateSkipUnknownField();
  void generateReadFieldEnd();

  void generateSwitchByFieldId();
  void generateDefaultCase();
  void generateSwitchBody();

  void generateFieldCase(const t_field* f);
  void generateFieldCaseElse();
  void generateFieldCaseBody(const t_field* f);

  void generateReadByType(const t_type* t, const std::string& fieldName);

  void generateStructFieldRead(const std::string& fieldName, bool optional = false);
  void generateBaseTypeFieldRead(const t_type* t, const std::string& fieldName);
  void generateEnumFieldRead(const t_type* t, const std::string& fieldName);
  void generateListFieldRead(const t_type* t, const std::string& fieldName);
  void generateSetFieldRead(const t_type* t, const std::string& fieldName);
  void generateMapFieldRead(const t_type* t, const std::string& fieldName);

  void generateVariablesForFields();
  void generateVariableForField(const t_field* f);

  void generateDataValidation();
  void generateRequiredFieldsValidationCheck();
  void generateSpecialFieldsValidationCheck();
  void generateDataValidationCheck();

  std::string getVariableName(const t_field* f) const;
  std::string getPrefixedEnumName(const t_type* t) const;

  std::string getInitializedFieldName(const t_field* f) const;

  bool isAnyFieldRequired() const;
  bool isAnyFieldSpecial() const;
  static bool isRequired(const t_field* f);

  const t_type* getTrueType(const t_field* f) const;

  class ContainerReadGenerator;
  class ListReadGenerator;
  class SetReadGenerator;
  class MapReadGenerator;

  const t_struct* tStruct_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTREADGENERATOR_H_
