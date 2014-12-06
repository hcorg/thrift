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

#ifndef THRIFT_CLIENT_CLIENTBASE_H_
#define THRIFT_CLIENT_CLIENTBASE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <thrift/protocol/TProtocol.h>
#include <thrift/TApplicationException.h>

namespace apache {
namespace thrift {
namespace client {

class ClientBase {
public:
  using TProtocolPtr = std::shared_ptr<protocol::TProtocol>;
  explicit ClientBase(TProtocolPtr prot);
  ClientBase(TProtocolPtr iprot, TProtocolPtr oprot);
  virtual ~ClientBase();

protected:
  template <typename Args>
  void sendMessage(const std::string& functionName, protocol::TMessageType type, const Args& args) {
    const std::int32_t cseqid = 0;
    oprot_->writeMessageBegin(functionName, type, cseqid);

    args.write(oprot_);

    oprot_->writeMessageEnd();
    oprot_->getTransport()->writeEnd();
    oprot_->getTransport()->flush();
  }

  template <typename Result>
  void recvMessage(const std::string& functionName, Result& result) {
    std::int32_t rseqid = 0;
    std::string fname;
    protocol::TMessageType mtype;

    iprot_->readMessageBegin(fname, mtype, rseqid);
    if (mtype == protocol::T_EXCEPTION) {
      TApplicationException x;
      x.read(iprot_);
      iprot_->readMessageEnd();
      iprot_->getTransport()->readEnd();
      throw x;
    }
    if (mtype != protocol::T_REPLY) {
      iprot_->skip(protocol::T_STRUCT);
      iprot_->readMessageEnd();
      iprot_->getTransport()->readEnd();
    }
    if (fname != functionName) {
      iprot_->skip(protocol::T_STRUCT);
      iprot_->readMessageEnd();
      iprot_->getTransport()->readEnd();
    }

    result.read(iprot_);
    iprot_->readMessageEnd();
    iprot_->getTransport()->readEnd();
  }

private:
  protocol::TProtocol* iprot_;
  protocol::TProtocol* oprot_;

  TProtocolPtr iprotPtr_;
  TProtocolPtr oprotPtr_;
};
}
}
} // namespace apache::thrift::client

#endif // THRIFT_CLIENT_CLIENTBASE_H_
