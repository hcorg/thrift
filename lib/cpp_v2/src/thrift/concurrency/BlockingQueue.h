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

#ifndef THRIFT_CONCURRENCY_BLOCKINGQUEUE_H_
#define THRIFT_CONCURRENCY_BLOCKINGQUEUE_H_

#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace apache {
namespace thrift {
namespace concurrency {

template <typename Item>
class BlockingQueue {
public:
  BlockingQueue() : working_(true) {}

  void stop() {
    working_ = false;
    notEmpty_.notify_all();
  }

  void push(const Item& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(item);
    lock.unlock();
    notEmpty_.notify_one();
  }

  void push(Item&& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(item));
    lock.unlock();
    notEmpty_.notify_one();
  }

  bool tryPop(Item& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    notEmpty_.wait(lock, [this]() { return !working_ || !queue_.empty(); });
    if (!working_)
      return false;
    using std::swap;
    swap(item, queue_.front());
    queue_.pop();
    return true;
  }

private:
  std::atomic<bool> working_;
  std::queue<Item> queue_;
  std::mutex mutex_;
  std::condition_variable notEmpty_;
};
}
}
} // apache::thrift::concurrency

#endif // THRIFT_CONCURRENCY_BLOCKINGQUEUE_H_
