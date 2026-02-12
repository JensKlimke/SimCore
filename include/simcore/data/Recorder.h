#pragma once

#include <utility>
#include <vector>

#include "../ISynchronized.h"
#include "Registry.h"

namespace sim::data {

class Recorder : public ISynchronized {
 public:
  explicit Recorder(Registry* registry) : registry_(registry) {}

  void initialize(double initTime) override {
    ISynchronized::initialize(initTime);
    frames_.clear();
  }

  bool step(double simTime) override {
    if (!ISynchronized::step(simTime)) return false;
    frames_.emplace_back(simTime, registry_->capture());
    return true;
  }

  void terminate(double /*simTime*/) override {}

  std::size_t frameCount() const { return frames_.size(); }

  const std::pair<double, std::vector<char>>& frame(std::size_t i) const {
    return frames_.at(i);
  }

  void writeTo(std::ostream& os) const {
    for (const auto& [time, data] : frames_) {
      uint64_t frame_size = data.size();
      double t = time;
      os.write(reinterpret_cast<const char*>(&t), sizeof(t));
      os.write(reinterpret_cast<const char*>(&frame_size), sizeof(frame_size));
      os.write(data.data(), static_cast<std::streamsize>(data.size()));
    }
  }

 private:
  Registry* registry_;
  std::vector<std::pair<double, std::vector<char>>> frames_;
};

}  // namespace sim::data
