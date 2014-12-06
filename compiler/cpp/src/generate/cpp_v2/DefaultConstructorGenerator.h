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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_DEFAULTCONSTRUCTORGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_DEFAULTCONSTRUCTORGENERATOR_H_

#include <string>

#include "StructConstructorGenerator.h"

class t_const_value;
class t_list;
class t_map;
class t_set;
class t_type;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class DefaultConstructorGenerator : public StructConstructorGenerator {
public:
  DefaultConstructorGenerator(OutputGenerator& parent, const t_struct* tStruct);

  void generateArguments() override;
  void generateDefinitionBody() override;
  void generateFieldValue(const t_field* f) override;

  bool hasDefaultValue(const t_field* f) const;
  bool isOptional(const t_field* f) const;
  bool isSimpleType(const t_type* type) const;

  const t_type* getFieldType(const t_field* f) const;

private:
  void generateValueForSimpleType(const t_type* t, const t_const_value* v);
  void generateValueForBaseType(const t_const_value* v);
  void generateValueForEnumType(const t_type* e, const t_const_value* v);

  void generateBodyForMember(const t_field* f);

  void generateBodyForField(const std::string& parentPrefix,
                            const t_field* f,
                            const t_const_value* val);
  void generateBodyForType(const std::string& fieldPrefix,
                           const t_type* type,
                           const t_const_value* val);

  void generateStructInit(const std::string& fieldPrefix,
                          const t_struct* s,
                          const t_const_value* v);
  void generateStructFieldInit(const std::string& fieldPrefix,
                               const t_field* field,
                               const t_const_value* value);

  void generateListInit(const std::string& parentPrefix, const t_list* l, const t_const_value* v);
  void generateListElement(const std::string& listPrefix,
                           const t_type* elemType,
                           const t_const_value* v);

  void generateSetInit(const std::string& parentPrefix, const t_set* s, const t_const_value* v);
  void generateSetElement(const std::string& setPrefix,
                          const t_type* elemType,
                          const t_const_value* v);

  void generateMapInit(const std::string& parentPrefix, const t_map* m, const t_const_value* v);
  void generateMapElement(const std::string& mapPrefix,
                          const t_type* keyType,
                          const t_type* valueType,
                          const t_const_value* key,
                          const t_const_value* value);

  void generateTempItemInitialization(const std::string& name,
                                      const t_type* type,
                                      const t_const_value* value);

  void generateTempItemInitIfNeeded(const std::string& name,
                                    const t_type* type,
                                    const t_const_value* value);

  void generateSimpleValueOrMove(const std::string& name,
                                 const t_type* type,
                                 const t_const_value* value);

  void initializeOptionalComplexField(const t_field* f, const std::string& parentPrefix);

  void generateTypeName(const t_type* type);

  std::string getFieldPrefix(const t_field* f, const std::string& parentPrefix) const;

  std::string getStructFieldName(const t_const_value* v) const;
  const t_field* getStructField(const t_struct* s, const t_const_value* v) const;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_DEFAULTCONSTRUCTORGENERATOR_H_
