#ifndef THRIFT_PROCESSOR_TCONNECTIONINFO_H_
#define THRIFT_PROCESSOR_TCONNECTIONINFO_H_

namespace apache {
namespace thrift {
namespace processor {

struct TConnectionInfo {
  // The input and output protocols
  std::shared_ptr<protocol::TProtocol> input;
  std::shared_ptr<protocol::TProtocol> output;
  // The underlying transport used for the connection
  // This is the transport that was returned by TServerTransport::accept(),
  // and it may be different than the transport pointed to by the input and
  // output protocols.
  std::shared_ptr<transport::TTransport> transport;
};
}
}
} // apache::thrift::processor

#endif // THRIFT_PROCESSOR_TCONNECTIONINFO_H_
