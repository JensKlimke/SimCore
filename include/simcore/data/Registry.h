#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

namespace sim::data {

class Registry {
 public:
  struct Entry {
    void* ptr;
    std::size_t size;
    std::type_index type;
    bool is_pod;
  };

  template <typename T>
  void publish(const std::string& name, T* ptr) {
    constexpr bool pod = std::is_trivially_copyable_v<T>;
    entries_.insert_or_assign(
        name, Entry{static_cast<void*>(ptr), sizeof(T),
                    std::type_index(typeid(T)), pod});
  }

  template <typename T>
  T& get(const std::string& name) {
    auto it = entries_.find(name);
    if (it == entries_.end())
      throw std::invalid_argument("Registry: no entry \"" + name + "\"");
#ifndef NDEBUG
    if (it->second.type != std::type_index(typeid(T)))
      throw std::logic_error("Registry: type mismatch for \"" + name + "\"");
#endif
    return *static_cast<T*>(it->second.ptr);
  }

  template <typename T>
  const T& get(const std::string& name) const {
    auto it = entries_.find(name);
    if (it == entries_.end())
      throw std::invalid_argument("Registry: no entry \"" + name + "\"");
#ifndef NDEBUG
    if (it->second.type != std::type_index(typeid(T)))
      throw std::logic_error("Registry: type mismatch for \"" + name + "\"");
#endif
    return *static_cast<const T*>(it->second.ptr);
  }

  void* get(const std::string& name) {
    auto it = entries_.find(name);
    if (it == entries_.end())
      throw std::invalid_argument("Registry: no entry \"" + name + "\"");
    return it->second.ptr;
  }

  bool has(const std::string& name) const {
    return entries_.find(name) != entries_.end();
  }

  // Binary snapshot of all POD entries.
  // Format: [uint32 name_len][name bytes][uint32 data_size][data bytes] ...
  std::vector<char> capture() const {
    std::vector<char> buf;
    for (const auto& [name, entry] : entries_) {
      if (!entry.is_pod) continue;
      uint32_t name_len = static_cast<uint32_t>(name.size());
      uint32_t data_size = static_cast<uint32_t>(entry.size);
      auto pos = buf.size();
      buf.resize(pos + sizeof(name_len) + name_len + sizeof(data_size) +
                 data_size);
      char* dst = buf.data() + pos;
      std::memcpy(dst, &name_len, sizeof(name_len));
      dst += sizeof(name_len);
      std::memcpy(dst, name.data(), name_len);
      dst += name_len;
      std::memcpy(dst, &data_size, sizeof(data_size));
      dst += sizeof(data_size);
      std::memcpy(dst, entry.ptr, data_size);
    }
    return buf;
  }

  void restore(const std::vector<char>& buf) {
    const char* src = buf.data();
    const char* end = src + buf.size();
    while (src < end) {
      uint32_t name_len = 0;
      std::memcpy(&name_len, src, sizeof(name_len));
      src += sizeof(name_len);
      std::string name(src, name_len);
      src += name_len;
      uint32_t data_size = 0;
      std::memcpy(&data_size, src, sizeof(data_size));
      src += sizeof(data_size);
      auto it = entries_.find(name);
      if (it != entries_.end() && it->second.is_pod &&
          it->second.size == data_size) {
        std::memcpy(it->second.ptr, src, data_size);
      }
      src += data_size;
    }
  }

  std::ostream& streamTo(std::ostream& os) const {
    nlohmann::json j = nlohmann::json::object();
    for (const auto& [name, entry] : entries_) {
      // Serialize numeric POD scalars; everything else as hex or skip
      if (entry.type == std::type_index(typeid(double))) {
        j[name] = *static_cast<const double*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(float))) {
        j[name] = *static_cast<const float*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(int))) {
        j[name] = *static_cast<const int*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(unsigned int))) {
        j[name] = *static_cast<const unsigned int*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(long))) {
        j[name] = *static_cast<const long*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(unsigned long))) {
        j[name] = *static_cast<const unsigned long*>(entry.ptr);
      } else if (entry.type == std::type_index(typeid(bool))) {
        j[name] = *static_cast<const bool*>(entry.ptr);
      } else {
        j[name] = "<" + std::string(entry.type.name()) + ":" +
                   std::to_string(entry.size) + "B>";
      }
    }
    os << j.dump();
    return os;
  }

  const std::unordered_map<std::string, Entry>& entries() const {
    return entries_;
  }

  std::size_t size() const { return entries_.size(); }

  void clear() { entries_.clear(); }

 private:
  std::unordered_map<std::string, Entry> entries_;
};

}  // namespace sim::data
