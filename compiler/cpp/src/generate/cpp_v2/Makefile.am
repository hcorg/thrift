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

if WITH_TESTS
SUBDIRS += tests
endif

noinst_LIBRARIES = libgenerate_cpp_v2.a

libgenerate_cpp_v2_a_CPPFLAGS = -I$(top_builddir)/compiler/cpp/src
libgenerate_cpp_v2_a_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror

libgenerate_cpp_v2_a_SOURCES = \
	ArgsRefsStructBuilder.cpp \
	ArgsRefsStructBuilder.h \
	ArgsRefsStructMembersGenerator.cpp \
	ArgsRefsStructMembersGenerator.h \
	ArgsStructBuilder.cpp \
	ArgsStructBuilder.h \
	ArgsStructGenerator.h \
	ArgsStructMembersGenerator.cpp \
	ArgsStructMembersGenerator.h \
	AssignmentOperatorGenerator.cpp \
	AssignmentOperatorGenerator.h \
	ClassScopeGuard.h \
	ClientFunctionGenerator.cpp \
	ClientFunctionGenerator.h \
	CompleteStructMembersGenerator.cpp \
	CompleteStructMembersGenerator.h \
	ContentsGenerator.cpp \
	ContentsGenerator.h \
	CopyConstructorGenerator.cpp \
	CopyConstructorGenerator.h \
	DefaultConstructorGenerator.cpp \
	DefaultConstructorGenerator.h \
	EnumGenerator.cpp \
	EnumGenerator.h \
	EnumHashGenerator.cpp \
	EnumHashGenerator.h \
	FieldsGenerator.cpp \
	FieldsGenerator.h \
	FieldsRefsGenerator.cpp \
	FieldsRefsGenerator.h \
	FunctionArgsGenerator.cpp \
	FunctionArgsGenerator.h \
	FunctionGenerator.cpp \
	FunctionGenerator.h \
	HandlerFactoryGenerator.cpp \
	HandlerFactoryGenerator.h \
	HashGenerator.cpp \
	HashGenerator.h \
	HeaderFileGenerator.cpp \
	HeaderFileGenerator.h \
	IfaceGenerator.cpp \
	IfaceGenerator.h \
	IncludesGenerator.cpp \
	IncludesGenerator.h \
	IncludesScanner.cpp \
	IncludesScanner.h \
	MethodGenerator.cpp \
	MethodGenerator.h \
	MoveAssignmentOperatorGenerator.cpp \
	MoveAssignmentOperatorGenerator.h \
	MoveConstructorGenerator.cpp \
	MoveConstructorGenerator.h \
	NamespaceGuard.h \
	OperatorEqGenerator.cpp \
	OperatorEqGenerator.h \
	OperatorLtGenerator.cpp \
	OperatorLtGenerator.h \
	OperatorOstreamGenerator.cpp \
	OperatorOstreamGenerator.h \
	ProcessorRegisterDispatchesGenerator.cpp \
	ProcessorRegisterDispatchesGenerator.h \
	RecvFunctionGenerator.cpp \
	RecvFunctionGenerator.h \
	ResultRefFieldsGenerator.cpp \
	ResultRefFieldsGenerator.h \
	ResultRefStructBuilder.cpp \
	ResultRefStructBuilder.h \
	ResultRefStructGenerator.h \
	ResultRefStructMembersGenerator.cpp \
	ResultRefStructMembersGenerator.h \
	ResultStructBuilder.cpp \
	ResultStructBuilder.h \
	SendFunctionGenerator.cpp \
	SendFunctionGenerator.h \
	ServiceClientGenerator.cpp \
	ServiceClientGenerator.h \
	ServiceFunctionGenerator.cpp \
	ServiceFunctionGenerator.h \
	ServiceInternalObjectsGenerator.cpp \
	ServiceInternalObjectsGenerator.h \
	ServiceInternalStructBuilder.cpp \
	ServiceInternalStructBuilder.h \
	ServiceInternalStructGenerator.h \
	ServiceInternalStructGeneratorTempl.h \
	ServiceInternalStructListGenerator.cpp \
	ServiceInternalStructListGenerator.h \
	SingletonHandlerFactoryGenerator.cpp \
	SingletonHandlerFactoryGenerator.h \
	SourceFileGenerator.cpp \
	SourceFileGenerator.h \
	StructClearGenerator.cpp \
	StructClearGenerator.h \
	StructConstructorGenerator.cpp \
	StructConstructorGenerator.h \
	StructDeclarationGenerator.cpp \
	StructDeclarationGenerator.h \
	StructDefinitionGenerator.cpp \
	StructDefinitionGenerator.h \
	StructGenerator.cpp \
	StructGenerator.h \
	StructHashGenerator.cpp \
	StructHashGenerator.h \
	StructMembersGenerator.cpp \
	StructMembersGenerator.h \
	StructReadGenerator.cpp \
	StructReadGenerator.h \
	StructWriteGenerator.cpp \
	StructWriteGenerator.h \
	SwapGenerator.cpp \
	SwapGenerator.h \
	TypeGenerator.cpp \
	TypeGenerator.h \
	TypedefGenerator.cpp \
	TypedefGenerator.h \
	Utils.cpp \
	Utils.h
