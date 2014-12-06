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

#ifndef THRIFT_COMPILER_GENERATOR_CPP_V2_INCLUDESSCANNER_H_
#define THRIFT_COMPILER_GENERATOR_CPP_V2_INCLUDESSCANNER_H_

#include <set>
#include <string>

class t_type;
class t_base_type;
class t_list;
class t_set;
class t_map;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class IncludesScanner {
public:
  typedef std::set<std::string> UsedTypes;
  IncludesScanner();

  void scan(const t_type* type);

  bool isCstdintRequired() const { return cstdintRequired_; }
  bool isStringRequired() const { return stringRequired_; }
  bool isVectorRequired() const { return vectorRequired_; }
  bool isSetRequired() const { return setRequired_; }
  bool isMapRequired() const { return mapRequired_; }

  const UsedTypes& getUsedTypes() const { return usedTypes_; }

private:
  void scanBaseType(const t_base_type* type);
  void scanNamedType(const t_type* type);
  void scanListType(const t_list* type);
  void scanSetType(const t_set* type);
  void scanMapType(const t_map* type);

protected: // for test stubs
  void addUsedType(const std::string& name);

  bool cstdintRequired_;
  bool stringRequired_;
  bool vectorRequired_;
  bool setRequired_;
  bool mapRequired_;
  UsedTypes usedTypes_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATOR_CPP_V2_INCLUDESSCANNER_H_
