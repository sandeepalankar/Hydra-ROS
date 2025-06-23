/* -----------------------------------------------------------------------------
 * Copyright 2022 Massachusetts Institute of Technology.
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Research was sponsored by the United States Air Force Research Laboratory and
 * the United States Air Force Artificial Intelligence Accelerator and was
 * accomplished under Cooperative Agreement Number FA8750-19-2-1000. The views
 * and conclusions contained in this document are those of the authors and should
 * not be interpreted as representing the official policies, either expressed or
 * implied, of the United States Air Force or the U.S. Government. The U.S.
 * Government is authorized to reproduce and distribute reprints for Government
 * purposes notwithstanding any copyright notation herein.
 * -------------------------------------------------------------------------- */
#include "hydra_ros/input/ros_data_receiver.h"

#include <config_utilities/config.h>
#include <ianvs/node_handle_factory.h>
#include <rclcpp/qos.hpp>

namespace hydra {

inline std::string getNamespace(const std::string& ns, const std::string& name) {
  return ns.empty() ? "~/input/" + name : ns;
}

rclcpp::QoS QoSConfig::toQoS() const {
  rclcpp::QoS qos(depth);
  
  // reliability
  if (reliability == "reliable") {
    qos.reliable();
  } else if (reliability == "best_effort") {
    qos.best_effort();
  }
  
  // durability
  if (durability == "transient_local") {
    qos.transient_local();
  } else if (durability == "volatile") {
    qos.durability_volatile();
  }
  
  // history
  if (history == "keep_all") {
    qos.keep_all();
  } else if (history == "keep_last") {
    qos.keep_last(depth);
  }
  
  return qos;
}

void declare_config(QoSConfig& config) {
  using namespace config;
  name("QoSConfig");
  field(config.reliability, "reliability");
  field(config.durability, "durability");
  field(config.history, "history");
  field(config.depth, "depth");
}

void declare_config(RosDataReceiver::Config& config) {
  using namespace config;
  name("RosDataReceiver::Config");
  base<DataReceiver::Config>(config);
  field(config.ns, "ns");
  field(config.queue_size, "queue_size");
  field(config.qos, "qos");
}

RosDataReceiver::RosDataReceiver(const Config& config, const std::string& sensor_name)
    : DataReceiver(config, sensor_name),
      config(config),
      ns_(getNamespace(config.ns, sensor_name)) {}

}  // namespace hydra
