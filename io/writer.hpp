#pragma once

#include "base/sarray_binstream.hpp"
#include "core/executor/executor.hpp"
#include "core/partition/partition_manager.hpp"
#include "io/abstract_writer.hpp"

namespace xyz {

class Writer {
public:
  Writer(int qid, std::shared_ptr<Executor> executor, std::shared_ptr<PartitionManager> partition_manager,
         std::function<std::shared_ptr<AbstractWriter>()> writer_getter)
      : qid_(qid), executor_(executor), partition_manager_(partition_manager), writer_getter_(writer_getter) {}

  ~Writer() {}

  void Write(int collection_id, int part_id, std::string dest_url,
             std::function<void(SArrayBinStream bin)> finish_handle);

private:
  int qid_;
  std::shared_ptr<Executor> executor_;
  std::shared_ptr<PartitionManager> partition_manager_;
  std::function<std::shared_ptr<AbstractWriter>()> writer_getter_;
};

} // namespace xyz
