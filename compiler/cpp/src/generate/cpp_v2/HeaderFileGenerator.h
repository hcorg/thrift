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

#ifndef THRIFT_COMPILER_GENERATE_CPP_V2_HEADERFILEGENERATOR_H_
#define THRIFT_COMPILER_GENERATE_CPP_V2_HEADERFILEGENERATOR_H_

#include <iosfwd>
#include <string>
#include <type_traits>

#include <generate/OutputGenerator.h>

#include "ContentsGenerator.h"

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

class HeaderFileGenerator : public OutputGenerator {
public:
  HeaderFileGenerator(std::ostream& out,
                      const std::string& moduleName,
                      const std::string& thriftNamespaceName);

  template <typename InnerGenerator, typename... Args>
  void generate(Args... args) {
    static_assert(std::is_base_of<ContentsGenerator, InnerGenerator>::value,
                  "template argument must inherit from ContentsGenerator");
    InnerGenerator gen(*this, args...);
    generate(gen);
  }

private:
  void generate(ContentsGenerator& inner_generator);

  void generateHeader(ContentsGenerator& innerGenerator);
  void generateContents(ContentsGenerator& innerGenerator);
  void generateFooter(ContentsGenerator& innerGenerator);

  std::string getIfdefOpen() const;
  std::string getIfdefClose() const;
  std::string getDefineName() const;

  const std::string moduleName_;
  const std::string thriftNamespaceName_;
};
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATE_CPP_V2_HEADERFILEGENERATOR_H_
