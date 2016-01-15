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

#ifndef THRIFT_COMPILER_GENERATE_T_CPP_V1_GENERATOR_H_
#define THRIFT_COMPILER_GENERATE_T_CPP_V1_GENERATOR_H_

#include "t_oop_generator.h"

class t_cpp_v1_generator : public t_oop_generator {
public:
  t_cpp_v1_generator(t_program* program,
                     const std::map<std::string, std::string>& parsed_options,
                     const std::string& option_string);

  void init_generator();
  void close_generator();

  void generate_consts(std::vector<t_const*> consts);
  void generate_typedef(t_typedef* ttypedef);
  void generate_enum(t_enum* tenum);
  void generate_forward_declaration(t_struct* tstruct);
  void generate_struct(t_struct* tstruct) { generate_cpp_struct(tstruct, false); }
  void generate_xception(t_struct* txception) { generate_cpp_struct(txception, true); }
  void generate_cpp_struct(t_struct* tstruct, bool is_exception);

  void generate_service(t_service* tservice);

  void print_const_value(std::ofstream& out, std::string name, t_type* type, t_const_value* value);
  std::string render_const_value(std::ofstream& out,
                                 std::string name,
                                 t_type* type,
                                 t_const_value* value);

  void generate_struct_declaration(std::ofstream& out,
                                   t_struct* tstruct,
                                   bool is_exception = false,
                                   bool pointers = false,
                                   bool read = true,
                                   bool write = true,
                                   bool swap = false,
                                   bool is_user_struct = false);
  void generate_struct_definition(std::ofstream& out,
                                  std::ofstream& force_cpp_out,
                                  t_struct* tstruct,
                                  bool setters = true);
  void generate_copy_constructor(std::ofstream& out, t_struct* tstruct, bool is_exception);
  void generate_move_constructor(std::ofstream& out, t_struct* tstruct, bool is_exception);
  void generate_constructor_helper(std::ofstream& out,
                                   t_struct* tstruct,
                                   bool is_excpetion,
                                   bool is_move);
  void generate_assignment_operator(std::ofstream& out, t_struct* tstruct);
  void generate_move_assignment_operator(std::ofstream& out, t_struct* tstruct);
  void generate_assignment_helper(std::ofstream& out, t_struct* tstruct, bool is_move);
  void generate_struct_reader(std::ofstream& out, t_struct* tstruct, bool pointers = false);
  void generate_struct_writer(std::ofstream& out, t_struct* tstruct, bool pointers = false);
  void generate_struct_result_writer(std::ofstream& out, t_struct* tstruct, bool pointers = false);
  void generate_struct_swap(std::ofstream& out, t_struct* tstruct);
  void generate_struct_print_method(std::ofstream& out, t_struct* tstruct);
  void generate_exception_what_method(std::ofstream& out, t_struct* tstruct);

  void generate_service_interface(t_service* tservice, std::string style);
  void generate_service_interface_factory(t_service* tservice, std::string style);
  void generate_service_null(t_service* tservice, std::string style);
  void generate_service_multiface(t_service* tservice);
  void generate_service_helpers(t_service* tservice);
  void generate_service_client(t_service* tservice, std::string style);
  void generate_service_processor(t_service* tservice, std::string style);
  void generate_service_skeleton(t_service* tservice);
  void generate_process_function(t_service* tservice,
                                 t_function* tfunction,
                                 std::string style,
                                 bool specialized = false);
  void generate_function_helpers(t_service* tservice, t_function* tfunction);
  void generate_service_async_skeleton(t_service* tservice);

  void generate_deserialize_field(std::ofstream& out,
                                  t_field* tfield,
                                  std::string prefix = "",
                                  std::string suffix = "");

  void generate_deserialize_struct(std::ofstream& out,
                                   t_struct* tstruct,
                                   std::string prefix = "",
                                   bool pointer = false);

  void generate_deserialize_container(std::ofstream& out, t_type* ttype, std::string prefix = "");

  void generate_deserialize_set_element(std::ofstream& out, t_set* tset, std::string prefix = "");

  void generate_deserialize_map_element(std::ofstream& out, t_map* tmap, std::string prefix = "");

  void generate_deserialize_list_element(std::ofstream& out,
                                         t_list* tlist,
                                         std::string prefix,
                                         bool push_back,
                                         std::string index);

  void generate_serialize_field(std::ofstream& out,
                                t_field* tfield,
                                std::string prefix = "",
                                std::string suffix = "");

  void generate_serialize_struct(std::ofstream& out,
                                 t_struct* tstruct,
                                 std::string prefix = "",
                                 bool pointer = false);

  void generate_serialize_container(std::ofstream& out, t_type* ttype, std::string prefix = "");

  void generate_serialize_map_element(std::ofstream& out, t_map* tmap, std::string iter);

  void generate_serialize_set_element(std::ofstream& out, t_set* tmap, std::string iter);

  void generate_serialize_list_element(std::ofstream& out, t_list* tlist, std::string iter);

  void generate_function_call(std::ostream& out,
                              t_function* tfunction,
                              std::string target,
                              std::string iface,
                              std::string arg_prefix);

  std::string namespace_prefix(std::string ns);
  std::string namespace_open(std::string ns);
  std::string namespace_close(std::string ns);
  std::string type_name(t_type* ttype, bool in_typedef = false, bool arg = false);
  std::string base_type_name(t_base_type::t_base tbase);
  std::string declare_field(t_field* tfield,
                            bool init = false,
                            bool pointer = false,
                            bool constant = false,
                            bool reference = false);
  std::string function_signature(t_function* tfunction,
                                 std::string style,
                                 std::string prefix = "",
                                 bool name_params = true);
  std::string cob_function_signature(t_function* tfunction,
                                     std::string prefix = "",
                                     bool name_params = true);
  std::string argument_list(t_struct* tstruct, bool name_params = true, bool start_comma = false);
  std::string type_to_enum(t_type* ttype);

  void generate_enum_constant_list(std::ofstream& f,
                                   const std::vector<t_enum_value*>& constants,
                                   const char* prefix,
                                   const char* suffix,
                                   bool include_values);

  void generate_struct_ostream_operator(std::ofstream& f, t_struct* tstruct);
  void generate_struct_print_method_decl(std::ofstream& f, t_struct* tstruct);
  void generate_exception_what_method_decl(std::ofstream& f,
                                           t_struct* tstruct,
                                           bool external = false);

  bool is_reference(t_field* tfield) { return tfield->get_reference(); }

  bool is_complex_type(t_type* ttype) {
    ttype = get_true_type(ttype);

    return ttype->is_container() || ttype->is_struct() || ttype->is_xception()
           || (ttype->is_base_type()
               && (((t_base_type*)ttype)->get_base() == t_base_type::TYPE_STRING));
  }

  void set_use_include_prefix(bool use_include_prefix) { use_include_prefix_ = use_include_prefix; }

private:
  /**
   * Returns the include prefix to use for a file generated by program, or the
   * empty string if no include prefix should be used.
   */
  std::string get_include_prefix(const t_program& program) const;

  /**
   * True if we should generate pure enums for Thrift enums, instead of wrapper classes.
   */
  bool gen_pure_enums_;

  /**
   * True if we should generate templatized reader/writer methods.
   */
  bool gen_templates_;

  /**
   * True iff we should generate process function pointers for only templatized
   * reader/writer methods.
   */
  bool gen_templates_only_;

  /**
   * True if we should generate move constructors & assignment operators.
   */
  bool gen_moveable_;

  /**
   * True iff we should use a path prefix in our #include statements for other
   * thrift-generated header files.
   */
  bool use_include_prefix_;

  /**
   * True if we should generate "Continuation OBject"-style classes as well.
   */
  bool gen_cob_style_;

  /**
   * True if we should omit calls to completion__() in CobClient class.
   */
  bool gen_no_client_completion_;

  /**
   * True if we should omit generating the default opeartors ==, != and <.
   */
  bool gen_no_default_operators_;

  /**
   * Strings for namespace, computed once up front then used directly
   */

  std::string ns_open_;
  std::string ns_close_;

  /**
   * File streams, stored here to avoid passing them as parameters to every
   * function.
   */

  std::ofstream f_types_;
  std::ofstream f_types_impl_;
  std::ofstream f_types_tcc_;
  std::ofstream f_header_;
  std::ofstream f_service_;
  std::ofstream f_service_tcc_;

  // The ProcessorGenerator is used to generate parts of the code,
  // so it needs access to many of our protected members and methods.
  //
  // TODO: The code really should be cleaned up so that helper methods for
  // writing to the output files are separate from the generator classes
  // themselves.
  friend class ProcessorGenerator;
};

#endif // THRIFT_COMPILER_GENERATE_T_CPP_V1_GENERATOR_H_
