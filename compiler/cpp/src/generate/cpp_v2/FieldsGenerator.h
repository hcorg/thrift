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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_FIELDSGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_FIELDSGENERATOR_H_

#include <vector>
#include <string>
#include <memory>

#include "ContentsGenerator.h"

class t_field;
class t_type;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class IncludesScanner;
class TypeGenerator;

class FieldsGenerator : public ContentsGenerator {
public:
  typedef std::vector<t_field*> Fields;
  FieldsGenerator(OutputGenerator& parent,
                  const Fields& fields,
                  const std::string& currentNamespace);
  ~FieldsGenerator();

  void generateContents() override;
  void generateHeader() override;

protected:
  virtual void generateField(const t_field* field);
  virtual void generateFieldType(const t_type* type);
  void generateTypeForField(const t_field* field);

private:
  void scanAllFieldsForHeaders();
  void scanForHeaders(const t_field* field);
  void scanForOptional(const t_field* field);
  void scanTypeForHeaders(const t_type* type);
  void prepareHeader();

  void generateOptionalFieldType(const t_type* type);

  void appendOptionalHeaderIfNeeded();
  void appendScannedHeaders();

  std::unique_ptr<IncludesScanner> scanner_;
  std::unique_ptr<TypeGenerator> typeGenerator_;

  const Fields& fields_;
  bool requireOptional_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_FIELDSGENERATOR_H_
