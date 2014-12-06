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

#include "SendFunctionGenerator.h"

#include <algorithm>

#include <parse/t_function.h>
#include <parse/t_service.h>
#include <parse/t_struct.h>

#include "ArgsRefsStructBuilder.h"
#include "ServiceClientGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;

SendFunctionGenerator::SendFunctionGenerator(OutputGenerator& parent,
                                             const t_service* service,
                                             const t_function* function)
  : ServiceFunctionGenerator(parent, ServiceClientGenerator::getClassName(service), function),
    service_(service) {
  setCurrentNamespace(getCppNamespace(service));
}

std::string SendFunctionGenerator::getGeneratedFunctionName() const {
  return "send_" + getFunction()->get_name();
}

void SendFunctionGenerator::generateReturnType() {
  out() << "void";
}

void SendFunctionGenerator::generateDefinitionBody() {
  generateArgumentsStruct();
  generateSendMessageCall();
}

void SendFunctionGenerator::generateSendMessageCall() {
  indented() << "sendMessage(\"" << getFunction()->get_name() << "\", " << getMessageType()
             << ", args);" << endl;
}

void SendFunctionGenerator::generateArgumentsStruct() {
  indented() << ArgsRefsStructBuilder(service_).buildName(getFunction()) << " args = {";
  generateArgumentsInitList();
  out() << "};" << endl;
}

std::string SendFunctionGenerator::getMessageType() const {
  return std::string("::apache::thrift::protocol::")
         + (getFunction()->is_oneway() ? "T_ONEWAY" : "T_CALL");
}

void SendFunctionGenerator::generateArgumentsInitList() {
  const auto args = getFunction()->get_arglist()->get_members();
  if (args.empty())
    return;

  out() << args.front()->get_name();
  std::for_each(args.begin() + 1, args.end(), [this](const t_field* s) {
    out() << ", " << s->get_name();
  });
}
