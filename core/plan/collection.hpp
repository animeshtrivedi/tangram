#pragma once

#include <vector>
#include <memory>
#include <sstream>

#include "core/plan/abstract_function_store.hpp"
#include "core/plan/collection_spec.hpp"
#include "core/scheduler/collection_view.hpp"

#include "core/index/abstract_key_to_part_mapper.hpp"
#include "core/partition/indexed_seq_partition.hpp"


namespace xyz {

struct CollectionBase {
  virtual ~CollectionBase() = default;
};

template<typename T, typename PartitionT = IndexedSeqPartition<T>>
class Collection : public CollectionBase {
 public:
  using ObjT = T;
  Collection(int id): Collection(id, 1) {}
  Collection(int id, int num_part): 
    id_(id), num_partition_(num_part) {
  }
  
  int Id() const {
    return id_;
  }

  // void SetWriteObj(std::function<void(ObjT& obj, std::stringstream& ss)> write_obj) {
  //   write_obj_ = write_obj;
  // }
  
  void SetMapper(std::shared_ptr<AbstractKeyToPartMapper> mapper) {
    mapper_ = mapper;
  }
  std::shared_ptr<AbstractKeyToPartMapper> GetMapper() {
    return mapper_;
  }

  // void RegisterWritePart(std::shared_ptr<AbstractFunctionStore> function_store) {
  //   function_store->AddWritePart(id_, [this](std::shared_ptr<AbstractPartition> part, 
  //                   std::shared_ptr<AbstractWriter> writer, std::string url) {
  //     std::stringstream ss;
  //     auto* p = static_cast<TypedPartition<ObjT>*>(part.get());
  //     CHECK_NOTNULL(p);
  //     for (auto& elem : *p) {
  //       write_obj_(elem, ss);
  //     }
  //     ss.seekg(0, std::ios::end);
  //     auto size = ss.tellg();
  //     writer->Write(url, ss.rdbuf(), size);  // TODO: test these lines
  //   });
  // }
 private:
  int id_;
  int num_partition_;
  CollectionSource source_ = CollectionSource::kOthers;
  // from distribute
  // std::vector<T> data_;
  // from hdfs file
  // std::string load_url_;
  // std::function<T(std::string&)> parse_line_;
  // std::function<void(ObjT&, std::stringstream& ss)> write_obj_;

  std::shared_ptr<AbstractKeyToPartMapper> mapper_;


};

}  // namespace

