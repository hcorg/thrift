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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_RESULTSTRUCTBUILDER_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_RESULTSTRUCTBUILDER_H_

#include <memory>
#include <string>

#include "ServiceInternalStructBuilder.h"

class t_field;
class t_struct;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class ResultStructBuilder : public ServiceInternalStructBuilder {
public:
  ResultStructBuilder(const t_service* service);
  ~ResultStructBuilder();

  const t_struct* build(const t_function* f) override;
  virtual std::string buildName(const t_function* f);

private:
  void appendAllExceptions(const t_function* f);
  void appendSuccessIfNotNull(const t_function* f);

  void appendField(t_field* f);

  std::unique_ptr<t_struct> struct_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_RESULTSTRUCTBUILDER_H_
