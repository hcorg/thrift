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

#ifndef THRIFT_COMPILER_GENERATE_T_CPP_V2_GENERATOR_H_
#define THRIFT_COMPILER_GENERATE_T_CPP_V2_GENERATOR_H_

#include "t_generator.h"

class t_cpp_v2_generator : public t_generator {
public:
  t_cpp_v2_generator(t_program* program,
                     const std::map<std::string, std::string>& parsed_options,
                     const std::string& option_string);

  //  TODO? virtual void generate_program();

protected:
  void init_generator() override;
  //  TODO? virtual void close_generator() {}

  //  TODO? virtual void generate_consts(std::vector<t_const*> consts);

  /**
   * Pure virtual methods implemented by the generator subclasses.
   */

  void generate_typedef(t_typedef* ttypedef) override;
  void generate_enum(t_enum* tenum) override;
  // TODO void generate_const(t_const* tconst) override;
  void generate_struct(t_struct* tstruct) override;
  void generate_service(t_service* tservice) override;

private:
  void create_output_dir();

  std::string get_filename_h(const std::string& base) const;
  std::string get_filename_cpp(const std::string& base) const;

  template <typename Generator, typename Type>
  void generateFiles(const Type* type);

  template <typename Generator, typename Type>
  void generateHeaderFile(const Type* type, const std::string& moduleName);

  template <typename Generator, typename Type>
  void generateSourceFile(const Type* type, const std::string& moduleName);

  template <typename Generator, typename Type>
  void generateHeaderOnly(const Type* type);
};

#endif // THRIFT_COMPILER_GENERATE_T_CPP_V2_GENERATOR_H_
