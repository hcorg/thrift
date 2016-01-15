#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#
#
# Contains some contributions under the Thrift Software License.
# Please see doc/old-thrift-license.txt in the Thrift distribution for
# details.

SUBDIRS = .

if HAVE_CPP11
SUBDIRS += cpp_v2
endif

if WITH_TESTS
SUBDIRS += tests
endif

noinst_LIBRARIES = libgenerate.a

libgenerate_a_CPPFLAGS = -I$(top_builddir)/compiler/cpp/src
libgenerate_a_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror

libgenerate_a_SOURCES = \
	t_generator.h \
	t_oop_generator.h \
	t_html_generator.h \
	t_generator.cc \
	t_generator_registry.h \
	t_generator.h \
	t_oop_generator.h \
	IndentKeeper.cpp \
	IndentKeeper.h \
	IndentGuard.h \
	PrePostGuard.h \
	ScopeGuard.h \
	OutputGenerator.h \
	OutputGenerator.cpp

# Language specific generators
libgenerate_a_SOURCES += \
	t_as3_generator.cc \
	t_c_glib_generator.cc \
	t_cocoa_generator.cc \
	t_cpp_generator.cc \
	t_cpp_v1_generator.cc \
	t_cpp_v1_generator.h \
	t_csharp_generator.cc \
	t_d_generator.cc \
	t_dart_generator.cc \
	t_delphi_generator.cc \
	t_erl_generator.cc \
	t_go_generator.cc \
	t_gv_generator.cc \
	t_haxe_generator.cc \
	t_hs_generator.cc \
	t_html_generator.cc \
	t_html_generator.h \
	t_java_generator.cc \
	t_javame_generator.cc \
	t_js_generator.cc \
	t_json_generator.cc \
	t_lua_generator.cc \
	t_ocaml_generator.cc \
	t_perl_generator.cc \
	t_php_generator.cc \
	t_py_generator.cc \
	t_rb_generator.cc \
	t_st_generator.cc \
	t_swift_generator.cc \
	t_xsd_generator.cc

if HAVE_CPP11
libgenerate_a_SOURCES += \
	t_cpp_v2_generator.h \
	t_cpp_v2_generator.cc
endif
