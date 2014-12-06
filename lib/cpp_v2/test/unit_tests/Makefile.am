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

check_PROGRAMS = CppV2UnitTests

TESTS = $(check_PROGRAMS)

ThriftTest_generated_sources = \
	gen-cpp/Bonk.cpp \
	gen-cpp/Bonk.h \
	gen-cpp/EmptyStruct.cpp \
	gen-cpp/EmptyStruct.h \
	gen-cpp/ListBonks.cpp \
	gen-cpp/ListBonks.h \
	gen-cpp/OneField.cpp \
	gen-cpp/OneField.h \
	gen-cpp/StructA.cpp \
	gen-cpp/StructA.h

OptionalRequiredTest_generated_sources = \
	gen-cpp/Tricky2.cpp \
	gen-cpp/Tricky2.h

TypedefTest_generated_sources = \
	gen-cpp/MyInt32.h \
	gen-cpp/MyString.h \
	gen-cpp/MyStruct.h \
	gen-cpp/TypedefTestStruct.cpp \
	gen-cpp/TypedefTestStruct.h

EnumTest_generated_source = \
	gen-cpp/MyEnum1.h \
	gen-cpp/MyEnum2.h \
	gen-cpp/MyEnum3.h \
	gen-cpp/MyEnum4.h \
	gen-cpp/MyEnumStruct.cpp \
	gen-cpp/MyEnumStruct.h

CppV2UnitTests_SOURCES = \
	$(ThriftTest_generated_sources) \
	$(OptionalRequiredTest_generated_sources) \
	$(TypedefTest_generated_sources) \
	$(EnumTest_generated_source) \
	processor/DispatcherTests.cpp \
	CppV2UnitTestsMain.cpp \
	EnumTests.cpp \
	ToStringTests.cpp \
	TypedefTests.cpp

CppV2UnitTests_LDADD = \
	-l:libboost_unit_test_framework.a \
	$(top_builddir)/lib/cpp_v2/libthrift2.la

THRIFT = $(top_builddir)/compiler/cpp/thrift

$(ThriftTest_generated_sources): $(top_srcdir)/test/ThriftTest.thrift $(THRIFT)
	$(THRIFT) --gen cpp_v2 $<

$(OptionalRequiredTest_generated_sources): $(top_srcdir)/test/OptionalRequiredTest.thrift $(THRIFT)
	$(THRIFT) --gen cpp_v2 $<

$(TypedefTest_generated_sources): $(top_srcdir)/test/TypedefTest.thrift $(THRIFT)
	$(THRIFT) --gen cpp_v2 $<

$(EnumTest_generated_source): $(top_srcdir)/test/EnumTest.thrift $(THRIFT)
	$(THRIFT) --gen cpp_v2 $<

AM_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
AM_CPPFLAGS = $(BOOST_CPPFLAGS) \
	-I$(top_builddir)/lib/cpp_v2/src
AM_LDFLAGS = $(BOOST_LDFLAGS)
