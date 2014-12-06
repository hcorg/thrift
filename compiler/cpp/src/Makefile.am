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

SUBDIRS = . ./generate/

AM_YFLAGS = -d
LIBS =
BUILT_SOURCES = thrifty.cc

noinst_LIBRARIES = libparse.a libparseobjects.a libcompilermd5.a

libparse_a_CPPFLAGS =
libparse_a_CXXFLAGS = -Wall -Wno-sign-compare -Wno-unused

libparse_a_SOURCES = thrifty.yy \
                     thriftl.ll

libcompilermd5_a_CPPFLAGS =
libcompilermd5_a_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror

libcompilermd5_a_SOURCES = \
	md5.c \
	md5.h

libparseobjects_a_CPPFLAGS = -I$(srcdir)/src
libparseobjects_a_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
libparseobjects_a_LIBADD = libcompilermd5.a

libparseobjects_a_SOURCES = \
	parse/t_doc.h \
	parse/t_type.h \
	parse/t_base_type.h \
	parse/t_base_type.cc \
	parse/t_enum.h \
	parse/t_enum_value.h \
	parse/t_typedef.h \
	parse/t_typedef.cc \
	parse/t_container.h \
	parse/t_list.h \
	parse/t_set.h \
	parse/t_map.h \
	parse/t_struct.h \
	parse/t_field.h \
	parse/t_service.h \
	parse/t_function.h \
	parse/t_program.h \
	parse/t_scope.h \
	parse/t_const.h \
	parse/t_const_value.h \
	parse/parse.cc

clean-local:
	$(RM) thriftl.cc thrifty.cc thrifty.h thrifty.hh

main.cc: version.h
