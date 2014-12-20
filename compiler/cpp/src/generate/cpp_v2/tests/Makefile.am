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

check_PROGRAMS = \
	CompilerCppV2GeneratorUnitTests

TESTS = \
	$(check_PROGRAMS)

CompilerCppV2GeneratorUnitTests_SOURCES = \
	ArgsRefsStructBuilderTests.cpp \
	ArgsRefsStructMembersGeneratorTests.cpp \
	ArgsStructBuilderTests.cpp \
	ArgsStructMembersGeneratorTests.cpp \
	AssignmentOperatorGeneratorTests.cpp \
	ClientFunctionGeneratorTests.cpp \
	CompilerCppV2UnitTestMain.cpp \
	CompleteStructMembersGeneratorTests.cpp \
	ContentsGeneratorStub.h \
	CopyConstructorGeneratorTests.cpp \
	DefaultConstructorGeneratorTests.cpp \
	EnumGeneratorTests.cpp \
	FieldsGeneratorTests.cpp \
	FieldsRefsGeneratorTests.cpp \
	FunctionArgsGeneratorTests.cpp \
	FunctionGeneratorTests.cpp \
	HandlerFactoryGeneratorTests.cpp \
	HeaderFileGeneratorTests.cpp \
	IfaceGeneratorTests.cpp \
	IncludesGeneratorTests.cpp \
	IncludesScannerTests.cpp \
	MethodGeneratorTests.cpp \
	MoveAssignmentOperatorGeneratorTests.cpp \
	MoveConstructorGeneratorTests.cpp \
	OperatorEqGeneratorTests.cpp \
	OperatorLtGeneratorTests.cpp \
	OperatorOstreamGeneratorTests.cpp \
	ProcessorRegisterDispatchesGeneratorTests.cpp \
	RecvFunctionGeneratorTests.cpp \
	ResultRefFieldsGeneratorTests.cpp \
	ResultRefStructMembersGeneratorTests.cpp \
	ResultStructBuilderTests.cpp \
	SendFunctionGeneratorTests.cpp \
	ServiceClientGeneratorTests.cpp \
	ServiceFunctionGeneratorTests.cpp \
	ServiceInternalObjectsGeneratorTests.cpp \
	ServiceInternalStructListGeneratorTests.cpp \
	SingletonHandlerFactoryGeneratorTests.cpp \
	SourceFileGeneratorTests.cpp \
	StructClearGeneratorTests.cpp \
	StructConstructorGeneratorTests.cpp \
	StructDeclarationGeneratorTests.cpp \
	StructDefinitionGeneratorTests.cpp \
	StructGeneratorTests.cpp \
	StructHashGeneratorTests.cpp \
	StructMembersGeneratorStub.h \
	StructReadGeneratorTests.cpp \
	StructWriteGeneratorTests.cpp \
	SwapGeneratorTests.cpp \
	TypeGeneratorTests.cpp \
	TypedefGeneratorTests.cpp \
	UtilsTests.cpp \
	parse_dummies.cpp

CompilerCppV2GeneratorUnitTests_LDADD = \
	$(BOOST_TEST_LDADD) \
	$(top_builddir)/compiler/cpp/src/libparseobjects.a \
	$(top_builddir)/compiler/cpp/src/libcompilermd5.a \
	$(top_builddir)/compiler/cpp/src/generate/libgenerate.a \
	$(top_builddir)/compiler/cpp/src/generate/cpp_v2/libgenerate_cpp_v2.a

AM_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
AM_CPPFLAGS = $(BOOST_CPPFLAGS) \
	-I$(top_builddir)/compiler/cpp/src
AM_LDFLAGS = $(BOOST_LDFLAGS)
