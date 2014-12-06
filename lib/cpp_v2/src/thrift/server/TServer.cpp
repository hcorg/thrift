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

#include "TServer.h"

#include <thrift/thrift-config.h>

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#include <thrift/protocol/TBinaryProtocol.h>

#ifdef HAVE_SYS_RESOURCE_H
int apache::thrift::server::increase_max_fds(int max_fds) {
  struct rlimit fdmaxrl;

  for (fdmaxrl.rlim_cur = max_fds, fdmaxrl.rlim_max = max_fds;
       max_fds && (setrlimit(RLIMIT_NOFILE, &fdmaxrl) < 0);
       fdmaxrl.rlim_cur = max_fds, fdmaxrl.rlim_max = max_fds) {
    max_fds /= 2;
  }

  return static_cast<int>(fdmaxrl.rlim_cur);
}
#endif

using namespace apache::thrift;
using namespace apache::thrift::server;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::processor::TSingletonProcessorFactory;

TServer::Args::Args(std::shared_ptr<TServerTransport> serverTransport)
  : serverTransport_(serverTransport) {
  setTransportFactory(std::make_shared<TTransportFactory>());
  setProtocolFactory(std::make_shared<TBinaryProtocolFactory>());
}

TServer::Args& TServer::Args::setServerTransport(std::shared_ptr<TServerTransport> t) {
  serverTransport_ = t;
  return *this;
}

TServer::Args& TServer::Args::setInputTransportFactory(std::shared_ptr<TTransportFactory> f) {
  inputTransportFactory_ = f;
  return *this;
}

TServer::Args& TServer::Args::setOutputTransportFactory(std::shared_ptr<TTransportFactory> f) {
  outputTransportFactory_ = f;
  return *this;
}

TServer::Args& TServer::Args::setTransportFactory(std::shared_ptr<TTransportFactory> f) {
  return setInputTransportFactory(f).setOutputTransportFactory(f);
}

TServer::Args& TServer::Args::setInputProtocolFactory(std::shared_ptr<TProtocolFactory> f) {
  inputProtocolFactory_ = f;
  return *this;
}

TServer::Args& TServer::Args::setOutputProtocolFactory(std::shared_ptr<TProtocolFactory> f) {
  outputProtocolFactory_ = f;
  return *this;
}

TServer::Args& TServer::Args::setProtocolFactory(std::shared_ptr<TProtocolFactory> f) {
  return setInputProtocolFactory(f).setOutputProtocolFactory(f);
}

TServer::Args& TServer::Args::setProcessor(std::shared_ptr<TProcessor> p) {
  return setProcessorFactory(std::make_shared<TSingletonProcessorFactory>(p));
}

TServer::Args& TServer::Args::setProcessorFactory(std::shared_ptr<TProcessorFactory> f) {
  processorFactory_ = f;
  return *this;
}

TServer::TServer(const Args& args) : args_(args) {
}

std::shared_ptr<TProcessor> TServer::buildProcessor(std::shared_ptr<TProtocol> inputProtocol,
                                                    std::shared_ptr<TProtocol> outputProtocol,
                                                    std::shared_ptr<TTransport> transport) {
  processor::TConnectionInfo connInfo;
  connInfo.input = inputProtocol;
  connInfo.output = outputProtocol;
  connInfo.transport = transport;
  return args_.processorFactory_->getProcessor(connInfo);
}

std::shared_ptr<TTransport> TServer::buildInputTransport(std::shared_ptr<TTransport> client) {
  return args_.inputTransportFactory_->getTransport(client);
}

std::shared_ptr<TTransport> TServer::buildOutputTransport(std::shared_ptr<TTransport> client) {
  return args_.outputTransportFactory_->getTransport(client);
}

std::shared_ptr<TProtocol> TServer::buildInputProtocol(std::shared_ptr<TTransport> transport) {
  return args_.inputProtocolFactory_->getProtocol(transport);
}

std::shared_ptr<TProtocol> TServer::buildOutputProtocol(std::shared_ptr<TTransport> transport) {
  return args_.outputProtocolFactory_->getProtocol(transport);
}

std::shared_ptr<TServerTransport> TServer::getServerTransport() {
  return args_.serverTransport_;
}
