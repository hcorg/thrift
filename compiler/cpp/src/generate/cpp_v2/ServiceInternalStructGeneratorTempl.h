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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATORTEMPL_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATORTEMPL_H_

#include <memory>

#include "ServiceInternalStructGenerator.h"
#include "ServiceInternalStructBuilder.h"
#include "StructMembersGenerator.h"
#include "StructDeclarationGenerator.h"
#include "StructDefinitionGenerator.h"

class t_struct;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

template <typename Builder, typename MembersGenerator>
class ServiceInternalStructGeneratorTempl : public ServiceInternalStructGenerator {
public:
  ServiceInternalStructGeneratorTempl(OutputGenerator& parent, const t_service* service)
    : ServiceInternalStructGenerator(parent), structBuilder_(new Builder(service)) {}

  void generateDeclaration(const t_function* function) override {
    StructDeclarationGenerator gen(*this, getMembersFactory(), buildStruct(function));
    gen.generateContents();
  }

  void generateDefinition(const t_function* function) override {
    StructDefinitionGenerator gen(*this, getMembersFactory(), buildStruct(function));
    gen.generateContents();
  }

private:
  std::unique_ptr<StructMembersGeneratorFactory> getMembersFactory() const {
    using Factory = StructMembersGeneratorFactoryTempl<MembersGenerator>;
    return std::unique_ptr<StructMembersGeneratorFactory>(new Factory);
  }

  const t_struct* buildStruct(const t_function* function) {
    return structBuilder_->build(function);
  }

  std::unique_ptr<ServiceInternalStructBuilder> structBuilder_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATORTEMPL_H_
