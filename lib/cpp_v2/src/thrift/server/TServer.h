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

#ifndef _THRIFT_SERVER_TSERVER_H_
#define _THRIFT_SERVER_TSERVER_H_ 1

#include <memory>

#include <thrift/processor/TProcessor.h>
#include <thrift/transport/TServerTransport.h>

namespace apache {
namespace thrift {
namespace server {

using apache::thrift::processor::TProcessor;
using apache::thrift::processor::TProcessorFactory;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::transport::TServerTransport;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TTransportFactory;

/**
 * Virtual interface class that can handle events from the server core. To
 * use this you should subclass it and implement the methods that you care
 * about. Your subclass can also store local data that you may care about,
 * such as additional "arguments" to these methods (stored in the object
 * instance's state).
 */
class TServerEventHandler {
public:
  virtual ~TServerEventHandler() {}

  /**
   * Called before the server begins.
   */
  virtual void preServe() {}

  /**
   * Called when a new client has connected and is about to being processing.
   */
  virtual void* createContext(std::shared_ptr<TProtocol> input, std::shared_ptr<TProtocol> output) {
    (void)input;
    (void)output;
    return NULL;
  }

  /**
   * Called when a client has finished request-handling to delete server
   * context.
   */
  virtual void deleteContext(void* serverContext) { (void)serverContext; }

  /**
   * Called when a client is about to call the processor.
   */
  virtual void preProcess(void* serverContext, std::shared_ptr<TTransport> transport) {
    (void)serverContext;
    (void)transport;
  }

protected:
  /**
   * Prevent direct instantiation.
   */
  TServerEventHandler() {}
};

/**
 * Thrift server.
 *
 */
class TServer {
public:
  struct Args {
    Args(std::shared_ptr<TServerTransport> serverTransport);

    Args& setInputTransportFactory(std::shared_ptr<TTransportFactory> factory);
    Args& setOutputTransportFactory(std::shared_ptr<TTransportFactory> factory);
    Args& setTransportFactory(std::shared_ptr<TTransportFactory> factory);

    Args& setInputProtocolFactory(std::shared_ptr<TProtocolFactory> f);
    Args& setOutputProtocolFactory(std::shared_ptr<TProtocolFactory> f);
    Args& setProtocolFactory(std::shared_ptr<TProtocolFactory> f);

    Args& setProcessor(std::shared_ptr<TProcessor> p);
    Args& setProcessorFactory(std::shared_ptr<TProcessorFactory> f);

    Args& setServerTransport(std::shared_ptr<TServerTransport> t);

    std::shared_ptr<TTransportFactory> inputTransportFactory_;
    std::shared_ptr<TTransportFactory> outputTransportFactory_;

    std::shared_ptr<TProtocolFactory> inputProtocolFactory_;
    std::shared_ptr<TProtocolFactory> outputProtocolFactory_;

    std::shared_ptr<TProcessorFactory> processorFactory_;

    std::shared_ptr<TServerTransport> serverTransport_;
  };

  virtual ~TServer() {}

  virtual void serve() = 0;

  virtual void stop() {}

  const Args& getArgs() const { return args_; }

  std::shared_ptr<TServerEventHandler> getEventHandler() { return eventHandler_; }

protected:
  TServer(const Args& args);

  /**
   * Get a TProcessor to handle calls on a particular connection.
   *
   * This method should only be called once per connection (never once per
   * call).  This allows the TProcessorFactory to return a different processor
   * for each connection if it desires.
   */
  std::shared_ptr<TProcessor> buildProcessor(std::shared_ptr<TProtocol> inputProtocol,
                                             std::shared_ptr<TProtocol> outputProtocol,
                                             std::shared_ptr<TTransport> transport);

  std::shared_ptr<TTransport> buildInputTransport(std::shared_ptr<TTransport> client);
  std::shared_ptr<TTransport> buildOutputTransport(std::shared_ptr<TTransport> client);

  std::shared_ptr<TProtocol> buildInputProtocol(std::shared_ptr<TTransport> transport);
  std::shared_ptr<TProtocol> buildOutputProtocol(std::shared_ptr<TTransport> transport);

  std::shared_ptr<TServerTransport> getServerTransport();

  Args args_;

  std::shared_ptr<TServerEventHandler> eventHandler_;

public:
  void setServerEventHandler(std::shared_ptr<TServerEventHandler> eventHandler) {
    eventHandler_ = eventHandler;
  }
};

/**
 * Helper function to increase the max file descriptors limit
 * for the current process and all of its children.
 * By default, tries to increase it to as much as 2^24.
 */
#ifdef HAVE_SYS_RESOURCE_H
int increase_max_fds(int max_fds = (1 << 24));
#endif
}
}
} // apache::thrift::server

#endif // #ifndef _THRIFT_SERVER_TSERVER_H_
