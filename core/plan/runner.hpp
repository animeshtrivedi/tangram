#pragma once

#include "core/plan/context.hpp"
#include "core/engine.hpp"
#include "core/program_context.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"


DECLARE_string(scheduler);
DECLARE_int32(scheduler_port);
DECLARE_string(hdfs_namenode);
DECLARE_int32(hdfs_port);
DECLARE_int32(num_local_threads);

namespace xyz {

class Runner {
 public:
  static void Init(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
  }
  static void Run() {

    CHECK(!FLAGS_scheduler.empty());

    auto plans = Context::get_allplans();
    auto collections = Context::get_allcollections();
    ProgramContext program;
    // for (auto* c : collections) {
    //   program.collections.push_back(c->GetSpec());
    // }
    for (auto* p : plans) {
      program.specs.push_back(p->GetSpec());
    }

    Engine::Config config;
    config.scheduler = FLAGS_scheduler;
    config.scheduler_port = FLAGS_scheduler_port;
    config.num_threads = FLAGS_num_local_threads;
    config.namenode = FLAGS_hdfs_namenode;
    config.port = FLAGS_hdfs_port;

    Engine engine;
    // initialize the components and actors,
    // especially the function_store, to be registed by the plan
    engine.Init(config);
    // register program containing plan and collection info
    engine.RegisterProgram(program);
    // add related functions
    // for (auto* c : collections) {
    //   engine.AddFunc(c);
    // }
    for (auto* p : plans) {
      engine.AddFunc(p);
    }

    // start the mailbox and start to receive messages
    engine.Start();
    // stop the mailbox and actors
    engine.Stop();
  }
};

} // namespace xyz

