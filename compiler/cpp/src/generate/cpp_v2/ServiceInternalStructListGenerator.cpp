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

#include "ServiceInternalStructListGenerator.h"

#include <algorithm>

#include <parse/t_service.h>

using namespace apache::thrift::compiler::generate::cpp_v2;

ServiceInternalStructListGenerator::ServiceInternalStructListGenerator(OutputGenerator& parent,
                                                                       const t_service* service)
  : OutputGenerator(parent), service_(service) {
}

template <typename Fun>
void ServiceInternalStructListGenerator::doForAllFunctionsForAllGenerators(Fun fun) {
  auto generators = buildGenerators();
  for (auto f : service_->get_functions())
    for (auto& gen : generators)
      fun(*gen, f);
}

void ServiceInternalStructListGenerator::generateDefinitions() {
  doForAllFunctionsForAllGenerators(
      [this](ServiceInternalStructGenerator& gen, const t_function* f) {
        gen.generateDefinition(f);
        out() << endl;
      });
}

void ServiceInternalStructListGenerator::generateDeclarations() {
  doForAllFunctionsForAllGenerators(
      [this](ServiceInternalStructGenerator& gen, const t_function* f) {
        gen.generateDeclaration(f);
        out() << endl;
      });
}

std::vector<ServiceInternalStructGeneratorPtr> ServiceInternalStructListGenerator::
    buildGenerators() {
  std::vector<ServiceInternalStructGeneratorPtr> generators;
  std::transform(factories_.begin(),
                 factories_.end(),
                 std::back_inserter(generators),
                 [this](ServiceInternalStructGeneratorFactoryPtr& f) {
    return f->createGenerator(*this, service_);
  });
  return generators;
}
