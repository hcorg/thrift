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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTWRITEGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTWRITEGENERATOR_H_

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

class StructWriteGenerator : public MethodGenerator {
public:
  StructWriteGenerator(OutputGenerator& parent, const t_struct* tstruct);

  void generateReturnType() override;
  void generateDeclarationWithoutReturn() override;
  void generateDefinitionBody() override;

public: // for tests
  void generateFieldWrite(const t_field* f);

private:
  void generateWriteHeader();
  void generateWriteFooter();
  void generateWriteFields();

  void generateFieldWriteSequence(const t_field* f);
  void generateFieldWriteBegin(const t_field* f);
  void generateFieldWriteEnd();

  void generateOptionalFieldWriteSequence(const t_field* f);
  void generateFieldWriteSequenceDependingOnReq(const t_field* f);

  void generateWriteByType(const t_type* t, const std::string& fieldName);

  void generateStructFieldWrite(const std::string& fieldName, bool optional = false);
  void generateBaseTypeFieldWrite(const t_type* t, const std::string& fieldName);
  void generateEnumFieldWrite(const std::string& fieldName);
  void generateListFieldWrite(const t_type* t, const std::string& fieldName);
  void generateSetFieldWrite(const t_type* t, const std::string& fieldName);
  void generateMapFieldWrite(const t_type* t, const std::string& fieldName);

  std::string getFieldName(const t_field* f) const;
  const t_type* getTrueType(const t_field* f) const;

  class ContainerWriteGenerator;
  class ListWriteGenerator;
  class SetWriteGenerator;
  class MapWriteGenerator;

  const t_struct* tStruct_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTWRITEGENERATOR_H_
