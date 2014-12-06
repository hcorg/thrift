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

#ifndef THRIFT_BOOSTOPTIONALUTILS_H_
#define THRIFT_BOOSTOPTIONALUTILS_H_

#include <boost/optional.hpp>

#include "TToString.h"

namespace apache {
namespace thrift {

template <typename T>
std::string to_string(const boost::optional<T>& t) {
  return t ? to_string(*t) : std::string("<null>");
}

template <typename T>
boost::optional<T>& initializedOptional(boost::optional<T>& o) {
  if (!o)
    o = T{};
  return o;
}

template <typename T>
boost::optional<T&>& initializedOptional(boost::optional<T&>& o) {
  return o;
}
}
} // apache::thrift

#endif // THRIFT_BOOSTOPTIONALUTILS_H_
