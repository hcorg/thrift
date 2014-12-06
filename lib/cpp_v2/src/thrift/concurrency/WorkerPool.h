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

#ifndef THRIFT_CONCURRENCY_WORKERPOOL_H_
#define THRIFT_CONCURRENCY_WORKERPOOL_H_

#include <memory>
#include <functional>

namespace apache {
namespace thrift {
namespace concurrency {

class WorkerPool {
public:
  using Task = std::function<void()>;

  WorkerPool(unsigned workerCount = 0);
  ~WorkerPool();

  void scheduleTask(const Task& task);
  void scheduleTask(Task&& task);

  void addWorker();
  void addWorkers(unsigned count);

  void stop();
  void joinAll();

  unsigned getWorkersCount() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};
}
}
} // apache::thrift::concurrency

#endif // THRIFT_CONCURRENCY_WORKERPOOL_H_
