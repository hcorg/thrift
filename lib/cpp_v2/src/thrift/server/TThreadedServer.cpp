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

#include "TThreadedServer.h"

#include <thread>
#include <string>
#include <iostream>

#include <thrift/transport/TTransportException.h>

namespace apache {
namespace thrift {
namespace server {

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::concurrency;

class TThreadedServer::Task {

public:
  Task(TThreadedServer& server,
       shared_ptr<TProcessor> processor,
       shared_ptr<TProtocol> input,
       shared_ptr<TProtocol> output,
       shared_ptr<TTransport> transport)
    : server_(server),
      processor_(processor),
      input_(input),
      output_(output),
      transport_(transport) {}

  ~Task() {}

  void run() {
    shared_ptr<TServerEventHandler> eventHandler = server_.getEventHandler();
    void* connectionContext = NULL;
    if (eventHandler) {
      connectionContext = eventHandler->createContext(input_, output_);
    }
    try {
      while (!server_.stop_) {
        if (eventHandler) {
          eventHandler->preProcess(connectionContext, transport_);
        }
        if (!processor_->process(input_, output_, connectionContext)
            || !input_->getTransport()->peek()) {
          break;
        }
      }
    } catch (const TTransportException& ttx) {
      if (ttx.getType() != TTransportException::END_OF_FILE) {
        string errStr = string("TThreadedServer client died: ") + ttx.what();
        GlobalOutput(errStr.c_str());
      }
    } catch (const std::exception& x) {
      GlobalOutput.printf("TThreadedServer exception: %s: %s", typeid(x).name(), x.what());
    } catch (...) {
      GlobalOutput("TThreadedServer uncaught exception.");
    }
    if (eventHandler) {
      eventHandler->deleteContext(connectionContext);
    }

    try {
      input_->getTransport()->close();
    } catch (TTransportException& ttx) {
      string errStr = string("TThreadedServer input close failed: ") + ttx.what();
      GlobalOutput(errStr.c_str());
    }
    try {
      output_->getTransport()->close();
    } catch (TTransportException& ttx) {
      string errStr = string("TThreadedServer output close failed: ") + ttx.what();
      GlobalOutput(errStr.c_str());
    }
  }

private:
  TThreadedServer& server_;
  friend class TThreadedServer;

  shared_ptr<TProcessor> processor_;
  shared_ptr<TProtocol> input_;
  shared_ptr<TProtocol> output_;
  shared_ptr<TTransport> transport_;
};

TThreadedServer::TThreadedServer(const Args& args) : TServer(args), stop_(false) {
}

TThreadedServer::~TThreadedServer() {
}

void TThreadedServer::stop() {
  stop_ = true;
  getServerTransport()->interrupt();
  workers_.stop();
}

void TThreadedServer::setMaxConcurrentConnections(unsigned maxConcurrentConnections) {
  assert(workers_.getWorkersCount() == 0);
  workers_.addWorkers(maxConcurrentConnections);
}

void TThreadedServer::serve() {
  if (workers_.getWorkersCount() == 0)
    setMaxConcurrentConnections(std::thread::hardware_concurrency());

  shared_ptr<TTransport> client;
  shared_ptr<TTransport> inputTransport;
  shared_ptr<TTransport> outputTransport;
  shared_ptr<TProtocol> inputProtocol;
  shared_ptr<TProtocol> outputProtocol;

  // Start the server listening
  getServerTransport()->listen();

  // Run the preServe event
  if (eventHandler_) {
    eventHandler_->preServe();
  }

  while (!stop_) {
    try {
      client.reset();
      inputTransport.reset();
      outputTransport.reset();
      inputProtocol.reset();
      outputProtocol.reset();

      // Fetch client from server
      client = getServerTransport()->accept();

      // Make IO transports
      inputTransport = buildInputTransport(client);
      outputTransport = buildOutputTransport(client);
      inputProtocol = buildInputProtocol(inputTransport);
      outputProtocol = buildOutputProtocol(outputTransport);

      shared_ptr<TProcessor> processor = buildProcessor(inputProtocol, outputProtocol, client);

      auto task = std::make_shared<TThreadedServer::Task>(*this,
                                                          processor,
                                                          inputProtocol,
                                                          outputProtocol,
                                                          client);
      workers_.scheduleTask([task]() { task->run(); });
    } catch (TTransportException& ttx) {
      if (inputTransport) {
        inputTransport->close();
      }
      if (outputTransport) {
        outputTransport->close();
      }
      if (client) {
        client->close();
      }
      if (!stop_ || ttx.getType() != TTransportException::INTERRUPTED) {
        string errStr = string("TThreadedServer: TServerTransport died on accept: ") + ttx.what();
        GlobalOutput(errStr.c_str());
      }
      continue;
    } catch (TException& tx) {
      if (inputTransport) {
        inputTransport->close();
      }
      if (outputTransport) {
        outputTransport->close();
      }
      if (client) {
        client->close();
      }
      string errStr = string("TThreadedServer: Caught TException: ") + tx.what();
      GlobalOutput(errStr.c_str());
      continue;
    } catch (string s) {
      if (inputTransport) {
        inputTransport->close();
      }
      if (outputTransport) {
        outputTransport->close();
      }
      if (client) {
        client->close();
      }
      string errStr = "TThreadedServer: Unknown exception: " + s;
      GlobalOutput(errStr.c_str());
      break;
    }
  }

  // If stopped manually, make sure to close server transport
  if (stop_) {
    try {
      getServerTransport()->close();
    } catch (TException& tx) {
      string errStr = string("TThreadedServer: Exception shutting down: ") + tx.what();
      GlobalOutput(errStr.c_str());
    }
    workers_.joinAll();
  }
}
}
}
} // apache::thrift::server
