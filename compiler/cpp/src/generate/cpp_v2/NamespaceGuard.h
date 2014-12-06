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

#ifndef THRIFT_COMPILER_GENERATOR_CPP_V2_NAMESPACEGUARD_H_
#define THRIFT_COMPILER_GENERATOR_CPP_V2_NAMESPACEGUARD_H_

#include <string>
#include <ostream>

#include "Utils.h"

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class NamespaceGuard {
public:
  NamespaceGuard(std::ostream& out, const std::string& thriftNamespaceName)
    : out_(out), name_(thriftNamespaceName) {
    out_ << namespaceOpen(name_) << "\n";
  }

  ~NamespaceGuard() { out_ << namespaceClose(name_) << "\n"; }

  NamespaceGuard(const NamespaceGuard&) = delete;
  NamespaceGuard& operator=(const NamespaceGuard&) = delete;

private:
  std::ostream& out_;
  const std::string name_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATOR_CPP_V2_NAMESPACEGUARD_H_
