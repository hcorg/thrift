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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_SINGLETONHANDLERFACTORYGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_SINGLETONHANDLERFACTORYGENERATOR_H_

#include "ContentsGenerator.h"

class t_service;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class SingletonHandlerFactoryGenerator : public ContentsGenerator {
public:
  SingletonHandlerFactoryGenerator(OutputGenerator& parent, const t_service* service);

  void generateHeader() override;
  void generateContents() override;

  static std::string getFileNameBase(const t_service* service) { return getClassName(service); }
  static std::string getClassName(const t_service* service);

private:
  void generateClassBegin();
  void generateClassEnd();

  void generatePublicSection();
  void generateConstructor();
  void generateGetHandler();
  void generateReleaseHandler();

  void generatePrivateSection();

  std::string getClassName() const;
  std::string getParentClassName() const;

  const t_service* service_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_SINGLETONHANDLERFACTORYGENERATOR_H_
