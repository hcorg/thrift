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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTMEMBERSGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTMEMBERSGENERATOR_H_

#include <memory>
#include <functional>

#include <generate/OutputGenerator.h>

class t_struct;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class FunctionGenerator;

class StructMembersGenerator : public OutputGenerator {
public:
  StructMembersGenerator(OutputGenerator& parent, const t_struct* tStruct);

  virtual void generateDefinitions() = 0;

  virtual void generateBodyDeclarations() = 0;
  virtual void generateStandaloneDeclarations() = 0;
  virtual void generateOtherNamespaceDeclarations() = 0;

  virtual void generateFields();

protected:
  std::function<void(FunctionGenerator& gen)> appendDeclaration;
  std::function<void(FunctionGenerator& gen)> appendDefinition;

  template <typename Generator, typename Fun>
  void doWith(Fun fun) {
    Generator gen(*this, struct_);
    fun(gen);
  }

  const t_struct* getStruct() const { return struct_; }

private:
  const t_struct* struct_;
};

class StructMembersGeneratorFactory {
public:
  virtual ~StructMembersGeneratorFactory();

  using StructMembersGeneratorPtr = std::unique_ptr<StructMembersGenerator>;

  virtual StructMembersGeneratorPtr createGenerator(OutputGenerator& parent, const t_struct* s) = 0;
};

using StructMembersGeneratorFactoryPtr = std::unique_ptr<StructMembersGeneratorFactory>;

template <typename Gen>
class StructMembersGeneratorFactoryTempl : public StructMembersGeneratorFactory {
public:
  StructMembersGeneratorPtr createGenerator(OutputGenerator& parent, const t_struct* s) override {
    return StructMembersGeneratorPtr(new Gen(parent, s));
  }
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_STRUCTMEMBERSGENERATOR_H_
