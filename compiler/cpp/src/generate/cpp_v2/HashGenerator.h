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

#ifndef THRIFT_COMPILER_GENERATE_HASHGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_HASHGENERATOR_H_

#include <string>

#include <generate/OutputGenerator.h>

class t_type;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class HashGenerator : public OutputGenerator {
protected:
  HashGenerator(OutputGenerator& parent, const t_type* type);

public:
  void generate();

protected:
  virtual void generateOperatorBody() = 0;

private:
  void generateStruct();
  void generateOperator();
  std::string getTypeNameWithNamespace() const;

  const t_type* type_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_HASHGENERATOR_H_
