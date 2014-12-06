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

#include "IncludesGenerator.h"

#include <ostream>

#include "IncludesScanner.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

IncludesGenerator::IncludesGenerator(OutputGenerator& parent) : OutputGenerator(parent) {
}

void IncludesGenerator::generate(const IncludesScanner& scanner) {
  generateStandardHeaders(scanner);
  generateTypeHeaders(scanner);
}

void IncludesGenerator::generateStandardHeaders(const IncludesScanner& scanner) {
  if (scanner.isCstdintRequired())
    indented() << systemInclude("cstdint") << endl;
  if (scanner.isStringRequired())
    indented() << systemInclude("string") << endl;
  if (scanner.isMapRequired())
    indented() << systemInclude("unordered_map") << endl;
  if (scanner.isVectorRequired())
    indented() << systemInclude("vector") << endl;
  if (scanner.isSetRequired())
    indented() << systemInclude("unordered_set") << endl;
}

void IncludesGenerator::generateTypeHeaders(const IncludesScanner& scanner) {
  for (const auto& type : scanner.getUsedTypes())
    indented() << localInclude(type + ".h") << endl;
}
