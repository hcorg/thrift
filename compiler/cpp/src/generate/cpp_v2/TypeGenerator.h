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

#ifndef THRIFT_COMPILER_GENERATOR_CPP_V2_TYPEGENERATOR_H_
#define THRIFT_COMPILER_GENERATOR_CPP_V2_TYPEGENERATOR_H_

#include <string>

#include <generate/OutputGenerator.h>

class t_type;
class t_base_type;
class t_set;
class t_map;
class t_list;
class t_container;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class TypeGenerator : public OutputGenerator {
public:
  explicit TypeGenerator(OutputGenerator& parent);

  void setCurrentNamespace(const std::string& ns);

  void generate(const t_type* type);

private:
  void throwIfNull(const t_type* type);
  void throwOnUnknownType(const t_type* type);
  void generateBaseType(const t_base_type* type);
  void generateListType(const t_list* type);
  void generateSetType(const t_set* type);
  void generateMapType(const t_map* type);
  void generateNamedType(const t_type* type);

  void generateSubsitutedType(const t_container* cont);

  std::string getOptionalNamespacePrefix(const t_type* type);

  std::string currentNamespace_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATOR_CPP_V2_TYPEGENERATOR_H_
