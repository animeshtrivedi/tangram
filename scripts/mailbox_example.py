#!/usr/bin/env python

import sys
from launch_utils import launch_util

hostfile = "machinefiles/5nodes"
progfile = "debug/WorkerExample"
schedulerfile = "debug/SchedulerExample"

params = {
    #"hdfs_namenode" : "proj10",
    #"hdfs_namenode_port" : 9000,
    #"input" : "hdfs:///jasper/kdd12",
    "scheduler" : "proj10",
    "scheduler_port" : "33254",
    "num_worker" : 5,
}

env_params = (
  "GLOG_logtostderr=true "
  "GLOG_v=-1 "
  "GLOG_minloglevel=0 "
  # this is to enable hdfs short-circuit read (disable the warning info)
  # change this path accordingly when we use other cluster
  # the current setting is for proj5-10
  "LIBHDFS3_CONF=/data/opt/course/hadoop/etc/hadoop/hdfs-site.xml"
  )

launch_util(schedulerfile, progfile, hostfile, env_params, params, sys.argv)
