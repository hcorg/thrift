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

#include "t_cpp_v2_generator.h"

#include "platform.h"

#include "cpp_v2/EnumGenerator.h"
#include "cpp_v2/HandlerFactoryGenerator.h"
#include "cpp_v2/HeaderFileGenerator.h"
#include "cpp_v2/IfaceGenerator.h"
#include "cpp_v2/ServiceClientGenerator.h"
#include "cpp_v2/ServiceInternalObjectsGenerator.h"
#include "cpp_v2/SingletonHandlerFactoryGenerator.h"
#include "cpp_v2/SourceFileGenerator.h"
#include "cpp_v2/StructGenerator.h"
#include "cpp_v2/TypedefGenerator.h"
#include "cpp_v2/Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

t_cpp_v2_generator::t_cpp_v2_generator(t_program* program,
                                       const std::map<std::string, std::string>& parsed_options,
                                       const std::string& option_string)
  : t_generator(program) {
  (void)option_string;
  // TODO process_options
  (void)parsed_options;

  out_dir_base_ = "gen-cpp";
}

void t_cpp_v2_generator::init_generator() {
  create_output_dir();
}

void t_cpp_v2_generator::generate_typedef(t_typedef* ttypedef) {
  generateHeaderOnly<TypedefGenerator>(ttypedef);
}

void t_cpp_v2_generator::generate_enum(t_enum* tenum) {
  generateHeaderOnly<EnumGenerator>(tenum);
}

template <typename Generator, typename Type>
void t_cpp_v2_generator::generateFiles(const Type* type) {
  const auto moduleName = Generator::getFileNameBase(type);
  generateHeaderFile<typename Generator::HeaderGenerator>(type, moduleName);
  generateSourceFile<typename Generator::SourceGenerator>(type, moduleName);
}

template <typename Generator, typename Type>
void t_cpp_v2_generator::generateHeaderFile(const Type* type, const std::string& moduleName) {
  std::ofstream out(get_filename_h(moduleName));
  HeaderFileGenerator generator(out, moduleName, getCppNamespace(type));
  generator.generate<Generator>(type);
}

template <typename Generator, typename Type>
void t_cpp_v2_generator::generateSourceFile(const Type* type, const std::string& moduleName) {
  std::ofstream out(get_filename_cpp(moduleName));
  SourceFileGenerator generator(out, moduleName, getCppNamespace(type));
  generator.generate<Generator>(type);
}

template <typename Generator, typename Type>
void t_cpp_v2_generator::generateHeaderOnly(const Type* type) {
  const auto moduleName = Generator::getFileNameBase(type);
  generateHeaderFile<Generator>(type, moduleName);
}

// void t_cpp_v2_generator::generate_const(t_const* tconst) {
//   /* TODO */
// }

void t_cpp_v2_generator::generate_struct(t_struct* tstruct) {
  generateFiles<StructGenerator>(tstruct);
}

void t_cpp_v2_generator::generate_service(t_service* s) {
  /* TODO */
  generateHeaderOnly<IfaceGenerator>(s);
  generateHeaderOnly<HandlerFactoryGenerator>(s);
  generateHeaderOnly<SingletonHandlerFactoryGenerator>(s);
  if (!s->get_functions().empty()) {
    generateFiles<ServiceInternalObjectsGenerator>(s);
  }
  generateFiles<ServiceClientGenerator>(s);
}

void t_cpp_v2_generator::create_output_dir() {
  MKDIR(get_out_dir().c_str());
}
std::string t_cpp_v2_generator::get_filename_h(const std::string& base) const {
  return get_out_dir() + "/" + base + ".h";
}

std::string t_cpp_v2_generator::get_filename_cpp(const std::string& base) const {
  return get_out_dir() + "/" + base + ".cpp";
}

THRIFT_REGISTER_GENERATOR(cpp_v2,
                          "C++ Version 2",
                          "   Generates files compatible with Version 2 of Thrift C++ library\n")
// "    cob_style:       Generate \"Continuation OBject\"-style classes.\n"
// "    no_client_completion:\n"
// "                     Omit calls to completion__() in CobClient class.\n"
// "    no_default_operators:\n"
// "                     Omits generation of default operators ==, != and <\n"
// "    templates:       Generate templatized reader/writer methods.\n"
// "    pure_enums:      Generate pure enums instead of wrapper classes.\n"
// "    dense:           Generate type specifications for the dense protocol.\n"
// "    include_prefix:  Use full include paths in generated files.\n"
// )
