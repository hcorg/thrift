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
AUTOMAKE_OPTIONS = subdir-objects serial-tests

noinst_LTLIBRARIES = libtutorialgencpp2.la
nodist_libtutorialgencpp2_la_SOURCES = \
	gen-cpp/CalculatorClient.cpp \
	gen-cpp/CalculatorClient.h \
	gen-cpp/CalculatorInternalObjects.cpp \
	gen-cpp/CalculatorInternalObjects.h \
	gen-cpp/InvalidOperation.cpp \
	gen-cpp/InvalidOperation.h \
	gen-cpp/MyInteger.h \
	gen-cpp/Operation.h \
	gen-cpp/SharedServiceClient.cpp \
	gen-cpp/SharedServiceClient.h \
	gen-cpp/SharedServiceInternalObjects.cpp \
	gen-cpp/SharedServiceInternalObjects.h \
	gen-cpp/SharedStruct.cpp \
	gen-cpp/SharedStruct.h \
	gen-cpp/Work.cpp \
	gen-cpp/Work.h

libtutorialgencpp2_la_LIBADD = $(top_builddir)/lib/cpp_v2/libthrift2.la

noinst_PROGRAMS = \
	TutorialClient
#	TODO TutorialServer

# TutorialServer_SOURCES = \
# 	CppServer.cpp

# TutorialServer_LDADD = \
# 	libtutorialgencpp.la \
# 	$(top_builddir)/lib/cpp/libthrift.la

TutorialClient_SOURCES = \
	CppClient.cpp

TutorialClient_LDADD = \
	libtutorialgencpp2.la

#
# Common thrift code generation rules
#
THRIFT = $(top_builddir)/compiler/cpp/thrift

gen-cpp/CalculatorClient.cpp gen-cpp/CalculatorClient.h gen-cpp/CalculatorInternalObjects.cpp gen-cpp/CalculatorInternalObjects.h gen-cpp/InvalidOperation.cpp gen-cpp/InvalidOperation.h gen-cpp/MyInteger.h gen-cpp/Operation.h gen-cpp/Work.cpp gen-cpp/Work.h: $(top_srcdir)/tutorial/tutorial.thrift
	$(THRIFT) --gen cpp_v2 $<

gen-cpp/SharedServiceClient.cpp gen-cpp/SharedServiceClient.h gen-cpp/SharedServiceInternalObjects.cpp gen-cpp/SharedServiceInternalObjects.h gen-cpp/SharedStruct.cpp gen-cpp/SharedStruct.h: $(top_srcdir)/tutorial/shared.thrift
	$(THRIFT) --gen cpp_v2 $<

AM_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/lib/cpp_v2/src -Igen-cpp
AM_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
AM_LDFLAGS = $(BOOST_LDFLAGS)

clean-local:
	$(RM) -r gen-cpp

# tutorialserver: all
# 	./TutorialServer

tutorialclient: all
	./TutorialClient

EXTRA_DIST = \
	CppClient.cpp
# CppServer.cpp

style-local:
	$(CPPSTYLE_CMD)
