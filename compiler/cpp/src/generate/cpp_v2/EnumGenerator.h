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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_ENUMGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_ENUMGENERATOR_H_

#include "ContentsGenerator.h"

#include <string>

class t_enum;
class t_enum_value;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class EnumGenerator : public ContentsGenerator {
public:
  EnumGenerator(OutputGenerator& parent, const t_enum* tenum);

  static std::string getFileNameBase(const t_enum* tenum);

  void generateContents() override;
  void generateHeader() override;
  void generateFooter() override;

private:
  void generateConstants();
  void generateConstant(const t_enum_value* constant);

  const t_enum* tEnum_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_ENUMGENERATOR_H_
