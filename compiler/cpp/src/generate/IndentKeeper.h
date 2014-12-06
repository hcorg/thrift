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

#ifndef THRIFT_COMPILER_GENERATE_INDENTKEEPER_H
#define THRIFT_COMPILER_GENERATE_INDENTKEEPER_H

#include <string>
#include <iosfwd>

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {

class IndentKeeper {
public:
  static const int INDENT_SIZE = 2;

  IndentKeeper();
  virtual ~IndentKeeper();

  std::string indent() const;
  std::ostream& indent(std::ostream& o) {
    o << indent();
    return o;
  }

  void indent_up();
  void indent_down();

  void setIndent(int indent);
  int getIndent() const { return indent_; }

private:
  int indent_;
};
}
}
}
} // apache::thrift::compiler::generate

#endif // THRIFT_COMPILER_GENERATE_INDENTKEEPER_H
