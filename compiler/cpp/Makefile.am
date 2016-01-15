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

SUBDIRS = ./src/

bin_PROGRAMS = thrift

thrift_OBJDIR = obj

thrift_SOURCES = src/main.cc \
                 src/globals.h \
                 src/main.h \
                 src/platform.h \
                 src/logging.h \
                 src/dummy_registry.cpp \
                 src/windows/config.h \
                 src/windows/version.h

thrift_CPPFLAGS = -I$(srcdir)/src
thrift_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
thrift_LDADD = \
	@LEXLIB@ \
	src/libparse.a src/libparseobjects.a src/libcompilermd5.a src/libcompileraudit.a \
	$(top_builddir)/compiler/cpp/src/generate/libgenerate.a

if HAVE_CPP11
thrift_LDADD += $(top_builddir)/compiler/cpp/src/generate/cpp_v2/libgenerate_cpp_v2.a
endif

WINDOWS_DIST = \
             compiler.sln \
             compiler.vcxproj \
             compiler.vcxproj.filters

EXTRA_DIST = \
             coding_standards.md \
             README.md \
             CMakeLists.txt \
             $(WINDOWS_DIST)

style-local:
	$(CPPSTYLE_CMD)
