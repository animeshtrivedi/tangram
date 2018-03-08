#include "core/plan/runner.hpp"

DEFINE_string(scheduler, "", "The host of scheduler");
DEFINE_int32(scheduler_port, -1, "The port of scheduler");
DEFINE_string(hdfs_namenode, "proj10", "The namenode of hdfs");
DEFINE_int32(hdfs_port, 9000, "The port of hdfs");
DEFINE_int32(num_local_threads, 1, "# local_threads");

using namespace xyz;

struct ObjT {
  using KeyT = std::string;
  using ValT = int;
  ObjT() = default;
  ObjT(KeyT key) : a(key), b(0) {}
  KeyT Key() const { return a; }
  KeyT a;
  int b;
  friend SArrayBinStream& operator<<(xyz::SArrayBinStream& stream, const ObjT& obj) {
    stream << obj.a << obj.b;
    return stream;
  }
  friend SArrayBinStream& operator>>(xyz::SArrayBinStream& stream, ObjT& obj) {
    stream >> obj.a >> obj.b;
    return stream;
  }
};


int main(int argc, char** argv) {
  Runner::Init(argc, argv);
  auto c1 = Context::distribute(std::vector<std::string>{"b", "a", "n", "a", "n", "a"}, 1);

  // std::string s;
  // auto c3 = Context::load(s, [](const std::string&) { return ObjT(); });

  auto c2 = Context::placeholder<ObjT>(10);

  // mapjoin
  Context::mapjoin(c1, c2, 
    [](std::string word) {
      return std::pair<std::string, int>(word, 1);
    },
    [](ObjT* obj, int m) {
      obj->b += m;
      LOG(INFO) << "join result: " << obj->a << " " << obj->b;
    });
  
  // mappartjoin
  Context::mappartjoin(c1, c2, 
    [](TypedPartition<std::string>* p, AbstractMapProgressTracker* t) {
      std::vector<std::pair<std::string, int>> kvs;
      for (auto& elem : *p) {
        kvs.push_back({elem, 1});
      }
      return kvs;
    },
    [](ObjT* obj, int m) {
      obj->b += m;
      LOG(INFO) << "join result: " << obj->a << " " << obj->b;
    });

  // mappartwithjoin
  // map 1, with 2, join 3
  Context::mappartwithjoin(c1, c2, c2,
    [](TypedPartition<std::string>* p,
       TypedCache<ObjT>* typed_cache, 
       AbstractMapProgressTracker* t) {
      std::vector<std::pair<std::string, int>> kvs;
      for (auto& elem : *p) {
        kvs.push_back({elem, 1});
      }
      return kvs;
    },
    [](ObjT* obj, int m) {
      obj->b += m;
      LOG(INFO) << "join result: " << obj->a << " " << obj->b;
    });

  // Context::write(c2, "", [](const ObjT& obj, std::stringstream& ss) {
  //   ss << obj.a << " " << obj.b << "\n";
  // });

  Runner::Run();
}
