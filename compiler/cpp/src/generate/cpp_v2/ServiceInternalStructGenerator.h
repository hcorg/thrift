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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATOR_H_

#include <memory>

#include <generate/OutputGenerator.h>

class t_function;
class t_service;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class ServiceInternalStructBuilder;
class StructMembersGeneratorFactory;

class ServiceInternalStructGenerator : public OutputGenerator {
public:
  ServiceInternalStructGenerator(OutputGenerator& parent) : OutputGenerator(parent) {}

  virtual void generateDeclaration(const t_function* function) = 0;
  virtual void generateDefinition(const t_function* function) = 0;
};

using ServiceInternalStructGeneratorPtr = std::unique_ptr<ServiceInternalStructGenerator>;

class ServiceInternalStructGeneratorFactory {
public:
  virtual ~ServiceInternalStructGeneratorFactory() {}

  virtual ServiceInternalStructGeneratorPtr createGenerator(OutputGenerator& parent,
                                                            const t_service* s) = 0;
};

using ServiceInternalStructGeneratorFactoryPtr
    = std::unique_ptr<ServiceInternalStructGeneratorFactory>;

template <typename Gen>
class ServiceInternalStructGeneratorFactoryTempl : public ServiceInternalStructGeneratorFactory {
public:
  ServiceInternalStructGeneratorPtr createGenerator(OutputGenerator& parent,
                                                    const t_service* s) override {
    return ServiceInternalStructGeneratorPtr(new Gen(parent, s));
  }
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_SERVICEINTERNALSTRUCTGENERATOR_H_
