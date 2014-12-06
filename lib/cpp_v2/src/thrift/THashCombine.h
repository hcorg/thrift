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

#ifndef THRIFT_THASHCOMBINE_H
#define THRIFT_THASHCOMBINE_H

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>

namespace apache {
namespace thrift {

template <typename T>
inline void hashCombine(std::size_t& seed, const T& v) {
  // based on: http://www.boost.org/doc/libs/1_53_0/doc/html/hash/reference.html#boost.hash_combine
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename K, typename V>
inline void hashCombine(std::size_t& seed, const std::unordered_map<K, V>& m) {
  for (const auto& item : m) {
    hashCombine(seed, item.first);
    hashCombine(seed, item.second);
  }
}

template <typename K, typename V>
inline void hashCombine(std::size_t& seed, const std::map<K, V>& m) {
  for (const auto& item : m) {
    hashCombine(seed, item.first);
    hashCombine(seed, item.second);
  }
}

template <typename K>
inline void hashCombine(std::size_t& seed, const std::set<K>& s) {
  for (const auto& item : s) {
    hashCombine(seed, item);
  }
}

template <typename K>
inline void hashCombine(std::size_t& seed, const std::unordered_set<K>& s) {
  for (const auto& item : s) {
    hashCombine(seed, item);
  }
}

template <typename K>
inline void hashCombine(std::size_t& seed, const std::vector<K>& v) {
  for (const auto& item : v) {
    hashCombine(seed, item);
  }
}
}
} // apache::thrift

#endif // THRIFT_HASHCOMBINE_H
