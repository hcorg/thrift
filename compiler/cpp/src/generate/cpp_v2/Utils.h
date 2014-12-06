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
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

#ifndef THRIFT_COMPILER_GENERATOR_CPP_V2_UTILS_H_
#define THRIFT_COMPILER_GENERATOR_CPP_V2_UTILS_H_

#include <string>

class t_field;
class t_base_type;
class t_type;

namespace apache {
namespace thrift {
namespace compiler {
namespace generate {
namespace cpp_v2 {

std::string getCppNamespace(const t_type* type);

std::string namespaceOpen(const std::string& thriftNamespaceName);
std::string namespaceClose(const std::string& thriftNamespaceName);
std::string namespaceName(const std::string& thriftNamespaceName);
std::string namespacePrefix(const std::string& thriftNamespaceName);
std::string namespacePrefixIfDifferent(const std::string& baseThriftNamespaceName,
                                       const std::string& otherThriftNamespaceName);

std::string namespaceOpen(const t_type* type);
std::string namespaceClose(const t_type* type);
std::string namespaceName(const t_type* type);
std::string namespacePrefix(const t_type* type);
std::string namespacePrefixIfDifferent(const t_type* referenceType, const t_type* prefixedType);

std::string autogenComment();

bool isUserDefinedType(const t_type* type);
bool isBasicType(const t_type* type);

std::string getTTypeString(const t_type* type);
std::string getReadWriteSuffix(const t_base_type* type);

std::string makeUniqueName(const std::string& nameTemplate);
void resetUniqueNameGenerator(); // for tests

std::string localInclude(const std::string& fileName);
std::string systemInclude(const std::string& fileName);

bool isResultSpecialField(const t_field* field);
}
}
}
}
} // apache::thrift::compiler::generate::cpp_v2

#endif // THRIFT_COMPILER_GENERATOR_CPP_V2_UTILS_H_
