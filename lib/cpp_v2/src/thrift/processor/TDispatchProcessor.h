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
#ifndef _THRIFT_TDISPATCHPROCESSOR_H_
#define _THRIFT_TDISPATCHPROCESSOR_H_ 1

#include <thrift/TProcessor.h>

namespace apache {
namespace thrift {

/**
 * TDispatchProcessor is a helper class to parse the message header then call
 * another function to dispatch based on the function name.
 *
 * Subclasses must implement dispatchCall() to dispatch on the function name.
 */
template <class Protocol>
class TDispatchProcessorT : public TProcessor {
public:
  virtual bool process(std::shared_ptr<protocol::TProtocol> in,
                       std::shared_ptr<protocol::TProtocol> out,
                       void* connectionContext) {
    protocol::TProtocol* inRaw = in.get();
    protocol::TProtocol* outRaw = out.get();

    // Try to dynamic cast to the template protocol type
    Protocol* specificIn = dynamic_cast<Protocol_*>(inRaw);
    Protocol* specificOut = dynamic_cast<Protocol_*>(outRaw);
    if (specificIn && specificOut) {
      return processFast(specificIn, specificOut, connectionContext);
    }

    // Log the fact that we have to use the slow path
    T_GENERIC_PROTOCOL(this, inRaw, specificIn);
    T_GENERIC_PROTOCOL(this, outRaw, specificOut);

    ProcessArgs args;
    std::string fname;
    protocol::TMessageType mtype;
    inRaw->readMessageBegin(fname, mtype, args.seqid);

    if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
      GlobalOutput.printf("received invalid message type %d from client", mtype);
      return false;
    }

    args.in = inRaw;
    args.out = outRaw;
    args.connectionContext = connectionContext;

    return this->dispatchCall(fname, args);
  }

protected:
  bool processFast(Protocol* in, Protocol* out, void* connectionContext) {
    ProcessArgsT<Protocol> args;
    std::string fname;
    protocol::TMessageType mtype;
    in->readMessageBegin(fname, mtype, args.seqid);

    if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
      GlobalOutput.printf("received invalid message type %d from client", mtype);
      return false;
    }

    args.in = in;
    args.out = out;
    args.connectionContext = connectionContext;

    return this->dispatchCallTemplated(fname, args);
  }

  /**
   * dispatchCall() methods must be implemented by subclasses
   */
  virtual bool dispatchCall(const std::string& funName, ProcessArgs args) = 0;

  virtual bool dispatchCallTemplated(const std::string& funName, ProcessArgs<Protocol> args) = 0;
};

/**
 * Non-templatized version of TDispatchProcessor, that doesn't bother trying to
 * perform a dynamic_cast.
 */
class TDispatchProcessor : public TProcessor {
public:
  virtual bool process(std::shared_ptr<protocol::TProtocol> in,
                       std::shared_ptr<protocol::TProtocol> out,
                       void* connectionContext) {
    ProcessArgs args;
    std::string fname;
    protocol::TMessageType mtype;
    in->readMessageBegin(fname, mtype, args.seqid);

    if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
      GlobalOutput.printf("received invalid message type %d from client", mtype);
      return false;
    }

    args.connectionContext = connectionContext;
    args.in = in.get();
    args.out = out.get();

    return dispatchCall(fname, args);
  }

  void processUnknownFunction(const std::string& funName, ProcessArgs args) {
    args.in->skip(protocol::T_STRUCT);
    args.in->readMessageEnd();
    args.in->getTransport()->readEnd();
    TApplicationException x(TApplicationException::UNKNOWN_METHOD,
                            "Invalid method name: '" + funName + "'");
    args.out->writeMessageBegin(funName, protocol::T_EXCEPTION, args.seqid);
    x.write(args.out);
    args.out->writeMessageEnd();
    args.out->getTransport()->writeEnd();
    args.out->getTransport()->flush();
  }

protected:
  virtual bool dispatchCall(const std::string& funName, ProcessArgs args) = 0;
};

// Specialize TDispatchProcessorT for TProtocol and TDummyProtocol just to use
// the generic TDispatchProcessor.
template <>
class TDispatchProcessorT<protocol::TDummyProtocol> : public TDispatchProcessor {};
template <>
class TDispatchProcessorT<protocol::TProtocol> : public TDispatchProcessor {};
}
} // apache::thrift

#endif // _THRIFT_TDISPATCHPROCESSOR_H_
