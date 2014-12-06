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

AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = .

if WITH_TESTS
SUBDIRS += test
endif

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libthrift2.la
pkgconfig_DATA = thrift.pc

AM_LDFLAGS = -release $(VERSION)
AM_CXXFLAGS = -Wall -Wextra -Wpedantic -Werror
AM_CPPFLAGS = -I$(srcdir)/src $(BOOST_CPPFLAGS)

if WITH_CPP_V2_ZLIB
lib_LTLIBRARIES += libthriftz2.la
pkgconfig_DATA += thrift-z.pc
endif

# Define the source files for the module

libthrift2_la_SOURCES = src/thrift/Thrift.cpp \
                        src/thrift/TApplicationException.cpp \
                        src/thrift/concurrency/WorkerPool.cpp \
                        src/thrift/client/ClientBase.cpp \
                        src/thrift/protocol/TDebugProtocol.cpp \
                        src/thrift/protocol/TDenseProtocol.cpp \
                        src/thrift/protocol/TJSONProtocol.cpp \
                        src/thrift/protocol/TBase64Utils.cpp \
                        src/thrift/protocol/TMultiplexedProtocol.cpp \
                        src/thrift/transport/TTransportException.cpp \
                        src/thrift/transport/TFDTransport.cpp \
                        src/thrift/transport/TFileTransport.cpp \
                        src/thrift/transport/TSimpleFileTransport.cpp \
                        src/thrift/transport/THttpTransport.cpp \
                        src/thrift/transport/THttpClient.cpp \
                        src/thrift/transport/THttpServer.cpp \
                        src/thrift/transport/TSocket.cpp \
                        src/thrift/transport/TPipe.cpp \
                        src/thrift/transport/TPipeServer.cpp \
                        src/thrift/transport/TSSLSocket.cpp \
                        src/thrift/transport/TSocketPool.cpp \
                        src/thrift/transport/TServerSocket.cpp \
                        src/thrift/transport/TSSLServerSocket.cpp \
                        src/thrift/transport/TTransportUtils.cpp \
                        src/thrift/transport/TBufferTransports.cpp \
                        src/thrift/server/TServer.cpp \
                        src/thrift/server/TSimpleServer.cpp \
                        src/thrift/server/TThreadedServer.cpp \
                        src/thrift/async/TAsyncChannel.cpp \
                        src/thrift/processor/PeekProcessor.cpp

libthriftz2_la_SOURCES = src/thrift/transport/TZlibTransport.cpp

include_thriftdir = $(includedir)/thrift
include_thrift_HEADERS = \
                         src/thrift/config.h \
                         src/thrift/thrift-config.h \
                         src/thrift/Thrift.h \
                         src/thrift/TReflectionLocal.h \
                         src/thrift/TApplicationException.h \
                         src/thrift/TLogging.h \
                         src/thrift/TToString.h \
                         src/thrift/BoostOptionalUtils.h \
                         src/thrift/THashCombine.h \
                         src/thrift/TException.h

include_concurrencydir = $(include_thriftdir)/concurrency
include_concurrency_HEADERS = \
                         src/thrift/concurrency/Exception.h \
                         src/thrift/concurrency/BlockingQueue.h \
                         src/thrift/concurrency/WorkerPool.h

include_clientdir = $(include_thriftdir)/client
include_client_HEADERS = \
                         src/thrift/client/ClientBase.h

include_protocoldir = $(include_thriftdir)/protocol
include_protocol_HEADERS = \
                         src/thrift/protocol/TBinaryProtocol.h \
                         src/thrift/protocol/TBinaryProtocol.tcc \
                         src/thrift/protocol/TCompactProtocol.h \
                         src/thrift/protocol/TCompactProtocol.tcc \
                         src/thrift/protocol/TDenseProtocol.h \
                         src/thrift/protocol/TDebugProtocol.h \
                         src/thrift/protocol/TBase64Utils.h \
                         src/thrift/protocol/TJSONProtocol.h \
                         src/thrift/protocol/TMultiplexedProtocol.h \
                         src/thrift/protocol/TProtocolDecorator.h \
                         src/thrift/protocol/TProtocolTap.h \
                         src/thrift/protocol/TProtocolException.h \
                         src/thrift/protocol/TVirtualProtocol.h \
                         src/thrift/protocol/TProtocol.h

include_transportdir = $(include_thriftdir)/transport
include_transport_HEADERS = \
                         src/thrift/transport/PlatformSocket.h \
                         src/thrift/transport/TFDTransport.h \
                         src/thrift/transport/TFileTransport.h \
                         src/thrift/transport/TSimpleFileTransport.h \
                         src/thrift/transport/TServerSocket.h \
                         src/thrift/transport/TSSLServerSocket.h \
                         src/thrift/transport/TServerTransport.h \
                         src/thrift/transport/THttpTransport.h \
                         src/thrift/transport/THttpClient.h \
                         src/thrift/transport/THttpServer.h \
                         src/thrift/transport/TSocket.h \
                         src/thrift/transport/TPipe.h \
                         src/thrift/transport/TPipeServer.h \
                         src/thrift/transport/TSSLSocket.h \
                         src/thrift/transport/TSocketPool.h \
                         src/thrift/transport/TVirtualTransport.h \
                         src/thrift/transport/TTransport.h \
                         src/thrift/transport/TTransportException.h \
                         src/thrift/transport/TTransportUtils.h \
                         src/thrift/transport/TBufferTransports.h \
                         src/thrift/transport/TZlibTransport.h \
                         src/thrift/transport/TShortReadTransport.h

include_serverdir = $(include_thriftdir)/server
include_server_HEADERS = \
                         src/thrift/server/TServer.h \
                         src/thrift/server/TSimpleServer.h \
                         src/thrift/server/TThreadedServer.h

include_processordir = $(include_thriftdir)/processor
include_processor_HEADERS = \
                         src/thrift/processor/Dispatcher.h \
                         src/thrift/processor/PeekProcessor.h \
                         src/thrift/processor/ProcessArgs.h \
                         src/thrift/processor/StatsProcessor.h \
                         src/thrift/processor/TConnectionInfo.h \
                         src/thrift/processor/TDispatchProcessor.h \
                         src/thrift/processor/TMultiplexedProcessor.h \
                         src/thrift/processor/TProcessor.h

include_asyncdir = $(include_thriftdir)/async
include_async_HEADERS = \
                     src/thrift/async/TAsyncChannel.h \
                     src/thrift/async/TAsyncDispatchProcessor.h \
                     src/thrift/async/TAsyncProcessor.h \
                     src/thrift/async/TAsyncBufferProcessor.h \
                     src/thrift/async/TAsyncProtocolProcessor.h \
                     src/thrift/async/TEvhttpClientChannel.h \
                     src/thrift/async/TEvhttpServer.h

THRIFT = $(top_builddir)/compiler/cpp/thrift

EXTRA_DIST = \
             README.md \
             thrift.pc.in

style-local:
	$(CPPSTYLE_CMD)

# Copied, so autotools will not add directory to -I if AC_CONFIG_HEADERS was used
src/thrift/config.h: $(top_builddir)/config.h
	cp $< $@
$(libthrift_la_SOURCES): src/thrift/config.h
