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

#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/CalculatorClient.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace tutorial;
using namespace shared;

int main() {
  auto socket = std::make_shared<TSocket>("localhost", 9090);
  auto transport = std::make_shared<TBufferedTransport>(socket);
  auto protocol = std::make_shared<TBinaryProtocol>(transport);
  CalculatorClient client(protocol);

  try {
    transport->open();

    client.ping();
    std::cout << "ping()" << std::endl;

    std::cout << "1 + 1 = " << client.add(1, 1) << std::endl;

    Work work;
    work.op = Operation::DIVIDE;
    work.num1 = 1;
    work.num2 = 0;

    try {
      client.calculate(1, work);
      std::cout << "Whoa? We can divide by zero!" << std::endl;
    } catch (InvalidOperation& io) {
      std::cout << "InvalidOperation: " << io.why << std::endl;
      // or using generated operator<<: std::cout << io << std::endl;
    }

    work.op = Operation::SUBTRACT;
    work.num1 = 15;
    work.num2 = 10;
    std::int32_t diff = client.calculate(1, work);
    std::cout << "15 - 10 = " << diff << std::endl;

    SharedStruct ss = client.getStruct(1);
    // Note: for costly move/copy operations
    //       C++ client provides also functions with return via reference for complex types
    //   SharedStruct ss;
    //   client.getStruct(ss, 1);

    std::cout << "Received log: " << ss << std::endl;

    transport->close();
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
}
